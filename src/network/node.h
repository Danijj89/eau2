//lang:CwC

#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include <mutex>
#include "NodeInfoArray.h"
#include "message.h"
#include "network.h"
#include "command_parser.h"
#include "../serdes/serializer.h"
#include "../serdes/deserializer.h"


/**
 * Node: Represents a node in the network.
 * A node can register to nodes, accept connections,
 * read from and write to other nodes directly.
 * The node spawns a new thread for each of the above operation.
 * The main thread listens to stdin for commands.
 *
 * @author: zhan.d@husky.neu.edu && yuan.cao1@husky.neu.edu
 */
class Node {
public:
	NodeInfo* info_; // the info of this node
	NodeInfoArray *node_infos_; // the other nodes in the network
	bool running_; // whether this node should be running
	std::thread** threads_; // the thread pool
	size_t size_; // curr num threads
	size_t max_capacity_; // max num threads
	std::mutex mutex; // lock for getting next thread index

	/**
	 * Constructs a new Node given its ip and port.
	 * @param ip the ip of the node
	 * @param port the port of the node
	 */
	Node(String *ip, int port) {
		int fd = get_new_fd();
		bind_fd_to_adr(ip, port, fd);
		this->info_ = new NodeInfo(ip, port, fd);
		this->node_infos_ = new NodeInfoArray();
		this->running_ = true;
		this->size_ = 0;
		this->max_capacity_ = MAX_NUM_THREADS;
		this->threads_ = new std::thread*[this->max_capacity_];
	}

	/**
	 * Default destructor
	 */
	virtual ~Node() {
		delete this->info_;
		delete this->node_infos_;
		for (size_t i = 0; i < this->size_; ++i) {
			delete this->threads_[i];
		}
		delete[] this->threads_;
	}

	/**
	 * Spawns a new thread to handle a connection on the given socket.
	 *
	 * @param fd the socket.
	 */
	void add_connection_handler(int fd) {
		size_t next_free_thread_idx = this->get_next_thread_index();
		this->threads_[next_free_thread_idx] = new std::thread(&Node::handle_connection, this, fd);
	}

	/**
	 * Spawns a new thread to handle incoming registration to this node.
	 */
	virtual void add_registration_handler() {
		size_t next_free_thread_idx = this->get_next_thread_index();
		this->threads_[next_free_thread_idx] = new std::thread(&Node::handle_registration, this);
	}

	/**
	 * Handles incoming registration.
	 */
	virtual void handle_registration() {
		while (this->running_) {
			this->register_connection();
		}
	}

	/**
	 * Spawns a new thread to handle a new accepted connection.
	 * If a connection was already established previously or if
	 * the node that is attempting to connect is not part of the
	 * network, the connection will be closed.
	 * @return success or failure.
	 */
	virtual bool register_connection() {
		NodeInfo* new_info = accept_connection(this->info_->get_fd());
		NodeInfo* curr_info = this->node_infos_->find(new_info);
		int new_fd = new_info->get_fd();
		if (curr_info != nullptr && curr_info->get_fd() == -1) {
			curr_info->set_fd(new_fd);
			this->add_connection_handler(new_fd);
			return true;
		}
		close(new_fd);
		delete new_info;
		return false;
	}

	/**
	 * Handles the commands coming from stdin.
	 */
	void handle_cli() {
		CommandParser* p = new CommandParser();
		char* buff = new char[MAX_BUFF_SIZE];
		while (this->running_) {
			read(0, buff, MAX_BUFF_SIZE);
			p->parse_command(buff);
			this->execute_command(p);
			printf("%s\n", buff);
		}
		delete p;
		delete[] buff;
	}

	/**
	 * TODO: can refactor the logic of each command to separate functions
	 * Executes the command stored in the given CommandParser.
	 * @param p the command parser
	 */
	virtual void execute_command(CommandParser* p) {
		Message* m;
		int target_fd;
		NodeInfo* info;
		switch(p->get_kind()) {
			case MsgKind::Register:
				this->connect_to_node(this->info_, p->get_target());
				break;
			case MsgKind::Put:
				info = this->node_infos_->find(p->get_target());
				assert(info != nullptr);
				target_fd = info->get_fd();
				assert(target_fd != -1);
				m = new Message(MsgKind::Empty);
				m->pack_body(p->get_data(), p->get_data_size(), SerDesTypes::NONE, 1);
				send_message(target_fd, m);
				delete m;
				break;
			case MsgKind::Shutdown:
				m = new Message(MsgKind::Shutdown);
				for (size_t i = 0; i < this->node_infos_->len(); ++i) {
					send_message(this->node_infos_->get(i)->get_fd(), m);
				}
				delete m;
				this->running_ = false;
				break;
			default:
				printf("Command not supported.\n");
				exit(1);
		}
	}

	void write_message(int socket, Message* m) {
		send_message(socket, m);
	}

	/**
	 * Starts a connection between the from and the to nodes.
	 * Spawn a new thread to handle the new connection.
	 * @param from
	 * @param to
	 */
	void connect_to_node(NodeInfo* from, NodeInfo* to) {
		int fd = connect_to(from, to);
		NodeInfo* local = this->node_infos_->find(to);
		if (local != nullptr) {
			local->set_fd(fd);
		}
		this->add_connection_handler(fd);
	}

	/**
	 * TODO: can refactor logic to process each message kind to its own method
	 * Handles an active connection by awaiting messages on a given socket.
	 * @param fd the socket.
	 */
	virtual void handle_connection(int fd) {
		Message* m = new Message();
		Serializer* ser = new Serializer();
		Deserializer* des = new Deserializer();
		while(this->running_) {
			read_message(fd, m);
			String* message = nullptr;
			NodeInfoArray* nia;
			switch(m->get_kind()) {
				case MsgKind::Directory:
					nia =  des->deserialize_node_info_array(m->get_body(), m->get_ser_member_size());
					for (size_t i = 0; i < nia->len(); ++i) {
						NodeInfo* curr = nia->get(i);
						if (this->node_infos_->find(curr) == nullptr) {
							this->node_infos_->pushBack(new NodeInfo(curr->get_ip()->clone(), curr->get_port()));
						}
					}
					printf("Directory from server with %ld addresses:\n%s", nia->len(), nia->toString()->c_str());
					delete nia;
					break;
				case MsgKind::Ack:
					message = des->deserialize_string(m->get_body());
					printf("%s\n", message->c_str());
					delete message;
					break;
				case MsgKind::Empty:
					printf("Received: %s\n", m->get_body());
					break;
				case MsgKind::Shutdown:
					this->running_ = false;
					break;
				default:
					assert(false);
			}
		}
		delete m;
		delete ser;
		delete des;
	}

	/**
	 * Makes sure that no data race can happen when allocating new threads
	 * @return
	 */
	size_t get_next_thread_index() {
		if (this->size_ >= this->max_capacity_) {
			perror("Reached max number of threads.");
			exit(1);
		}
		mutex.lock();
		size_t result = this->size_;
		this->size_++;
		mutex.unlock();
		return result;
	}

	/**
	 * Starts the node.
	 * Connects to the server, adds threads to handle
	 * incoming connections, and handle command line instructions.
	 */
	virtual void start() {
		NodeInfo* server_info = new NodeInfo(new String(SERVER_IP), PORT);
		connect_to_node(this->info_, server_info);
		this->add_registration_handler();
		this->handle_cli();
		delete server_info;
		delete this;
	}
};

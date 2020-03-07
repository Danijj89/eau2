//lang:CwC

#pragma once

#include <arpa/inet.h>
#include "helper.h"
#include "node.h"
#include "message.h"

/**
 * Server: represents the rendezvous server in the network.
 */
class Server : public Node {
public:

	Server(String* ip, int port) : Node(ip, port) {}

	void start() override {
		this->add_registration_handler();
		this->handle_cli();
	}

	void add_registration_handler() override {
		if (this->size_ == this->max_capacity_) {
			perror("Reached max number of threads.");
			exit(1);
		}
		this->threads_[this->size_] = new std::thread(&Server::handle_registration, this);
		this->size_++;
	}

	void handle_registration() override {
		while (this->running_) {
			if (this->register_connection()) {
				this->broadcast_directory();
			}
		}
	}

	bool register_connection() override {
		NodeInfo* new_node = accept_connection(this->info_->get_fd());
		if (new_node != nullptr) {
			this->node_infos_->pushBack(new_node);
			return true;
		}
		return false;
	}

	void broadcast_directory() {
		Message* m = new Message(MsgKind::Directory);
		Serializer* s = new Serializer();
		s->serialize_node_info_array(this->node_infos_);
		m->pack_body(s->get_buff(), s->get_size(), SerDesTypes::NODEINFOARRAY, this->node_infos_->len());
		for (size_t i = 0; i < this->node_infos_->len(); ++i) {
			send_message(this->node_infos_->get(i)->get_fd(), m);
		}
		delete m;
		delete s;
	}
};
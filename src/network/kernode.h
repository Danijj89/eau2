#include "../util/constants.h"
#include "../serdes/serializer.h"
#include "../serdes/deserializer.h"

#include "network.h"
#include "network2.h"
#include "NodeInfoArray.h"
#include "pollfds.h"

/**
 * Kernode: Represent the minimum capabilities of a node in a network.
 */
class Kernode {
public:
	NodeInfoArray* addressBook_;
	Pollfds* connections_;
	NodeInfo* myInfo_;
	size_t id_;
	bool running_;
	size_t maxNodes_ = NODES;

	Kernode(int nodeId) {
		String* ip;
		switch(nodeId) {
			case 0:
				ip = new String(SERVER_IP);
				break;
			case 1:
				ip = new String(NODE1_IP);
				break;
			case 2:
				ip = new String(NODE2_IP);
				break;
			case 3:
				ip = new String(NODE3_IP);
				break;
			case 4:
				ip = new String(NODE4_IP);
				break;
			default:
				assert (false);
		}
		this->myInfo_ = new NodeInfo(ip, PORT, 0);
		this->running_ = false;
		this->id_ = nodeId;
		printf("%lu: Initialized Node\n", this->id_);
	}

	virtual ~Kernode() {
		delete this->myInfo_;
		delete this->addressBook_;
		delete this->connections_;
		printf("%lu: Deleted Node\n", this->id_);
	}

	virtual void run() {
		printf("%lu: Started Run\n", this->id_);
		int listener = this->setupListener();
		printf("%lu: Got Listener\n", this->id_);
		if (this->id_ == 0) {
			this->setupServer(listener);
			printf("%lu: Set up server\n", this->id_);
		} else {
			printf("%lu: Ready to connect to network\n", this->id_);
			this->connectToNetwork(listener);
			printf("%lu: Connected to network\n", this->id_);
		}
		performDelegatedAction();
	}

	virtual int setupListener() {
		int listener = get_new_fd();
		bind_fd_to_adr(this->myInfo_->get_ip(), this->myInfo_->get_port(), listener);
		assert (listen(listener, BACKLOG) == 0);
		return listener;
	}

	virtual void setupServer(int listener) {
		this->connections_ = new Pollfds(listener, -1);
		this->addressBook_ = new NodeInfoArray();
		printf("%lu: Got necessary structs for server, and ready to accept\n", this->id_);
		this->acceptConnections(listener);
		this->broadcastAddressBook();
	}

	// Used by both server role and node role
	virtual void acceptConnections(int listener) {
		int expectedConnections = this->maxNodes_ - this->id_;
		printf("%lu: Obligated to accept %d connections\n", this->id_, expectedConnections);
		for (int i = 0; i < expectedConnections; ++i) {
			NodeInfo* info = accept_connection(listener);
			printf("%lu: Got a connection\n", this->id_);
			NodeInfo* previousInfo = this->addressBook_->find(info);
			if (previousInfo == nullptr) {
				this->addressBook_->pushBack(info);
				this->connections_->add_to_pfds(info->get_fd());
			} else {
				int newFd = info->get_fd();
				previousInfo->set_fd(newFd);
				this->connections_->add_to_pfds(newFd);
				delete info;
			}
		}
		printf("%lu: Accepted all obligated connects\n", this->id_);
	}

	virtual void broadcastAddressBook() {
		printf("%lu: Broadcasting address book\n", this->id_);
		Message* m = new Message(MsgKind::Directory);
		Serializer* s = new Serializer();
		s->serialize_node_info_array(this->addressBook_);
		m->pack_body(s->get_buff(), s->get_size(), SerDesTypes::NODEINFOARRAY, this->addressBook_->len());
		for (size_t i = 0; i < this->addressBook_->len(); ++i) {
			send_message(this->addressBook_->get(i)->get_fd(), m);
		}
		delete m;
		delete s;
		printf("%lu: Broadcasted address book\n", this->id_);
	}

	virtual void connectToNetwork(int listener) {
		printf("%lu: Connecting to server\n", this->id_);
		this->connectToServer(listener);
		printf("%lu: Connected to server\n", this->id_);
		this->connectToLesserNodes();
		printf("%lu: Connected to assigned nodes\n", this->id_);
		this->acceptConnections(listener);
		printf("%lu: Accepted to assigned nodes\n", this->id_);
	}

	virtual void connectToServer(int listener) {
		int server = connectToDefaultServer();
		this->connections_ = new Pollfds(listener, server);
		printf("%lu: Getting address book\n", this->id_);
		this->getAddressBook();
		printf("%lu: Got address book\n", this->id_);
	}

	virtual void getAddressBook() {
		Message* m = new Message();
		read_message(this->connections_->pfds()[2].fd, m);
		assert (m->get_kind() == MsgKind::Directory);
		size_t n = m->get_ser_member_size();
		assert (n == this->maxNodes_);
		Deserializer d = Deserializer();
		this->addressBook_ = d.deserialize_node_info_array(m->get_body(), n);
		delete m;
	}

	virtual void connectToLesserNodes() {
		for (size_t i = 1; i < NODES; ++i) {
			if (i == this->id_) {
				break;
			}
			printf("%lu: Connect to %lu\n", this->id_, i);
			NodeInfo* otherInfo = this->addressBook_->get(i);
			int fd = connectToNode(otherInfo);
			otherInfo->set_fd(fd);
			this->connections_->add_to_pfds(fd);
			printf("%lu: Connected to %lu\n", this->id_, i);
		}
	}

	virtual void performDelegatedAction() {
		switch (this->id_) {
			case 0: // perform server action
			case 1: // perform this->id_ action
			case 2:
			case 3:
			case 4:
			default:
				printf("%lu: !!!PERFORMING ACTION ASSIGNED!!!\n", this->id_);
				// this->loop();
				break;
		}
	}

	// Unused for now
	// This demonstrate how pollserver and client handle multiple connection
	virtual void loop() {
		char buf [1024] = {0};
		while(this->running_) {
			if (poll(this->connections_->pfds(), this->connections_->size(), -1) == -1) {
				fprintf(stderr, "Error with poll mechanism.\n");
			}

			for (size_t i = 0; i < this->connections_->size(); ++i) {
				if (this->connections_->pfds()[i].revents == POLLIN) {
					printf("KERNODE: found something...\n");
					if (i == 0) { // stdin
						read_from(0, buf, 1024);
						printf("KERNODE: %s\n", buf);
					} else if (i == 1) { // Listener port
						// Handle new connection
						printf("KERNODE: Got to 1\n");
					} else if (i == 2) { // Server connection
						// Handle new instructions from server
						printf("KERNODE: Got to 2\n");
					} else { // Every other connection
						// Handle requests from other nodes
						printf("KERNODE: Got to other\n");
					}
				}
			}
		}
	}
};

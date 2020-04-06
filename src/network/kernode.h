#pragma once



#include "network.h"
#include "NodeInfoArray.h"
#include "pollfds.h"
#include "../dataframe/column.h"
#include "../util/constants.h"
#include "../serdes/serializer.h"
#include "../serdes/deserializer.h"

/**
 * Kernode: Represent the minimum capabilities of a node in a network.
 */
class Kernode {
public:
	int id_;
	NodeInfoArray* addressBook_ = nullptr;
	Pollfds* pollfds_;
	NodeInfo* myInfo_;
	bool running_;
	size_t maxNodes_ = NUM_NODES;

	Kernode(int id, String* ip, int port) {
		this->id_ = id;
		this->pollfds_ = new Pollfds();
		this->myInfo_ = new NodeInfo(ip, port);
		this->running_ = true;
		printf("%d: Initialized Node\n", this->id_);
	}

	virtual ~Kernode() {
		delete this->myInfo_;
		delete this->addressBook_;
		delete this->pollfds_;
		printf("%d: Deleted Node\n", this->id_);
	}

	virtual void initialize() {
		printf("%d: Started Run\n", this->id_);
		
		// initialize listening socket for this node
		int listener = get_new_fd();
		bind_fd_to_adr(this->myInfo_->get_ip(), this->myInfo_->get_port(), listener);
		assert (listen(listener, BACKLOG) == 0);
		this->addressBook_->get(0)->set_fd(listener);

		printf("%d: Got Listener\n", this->id_);
		if (this->id_ == SERVER_ID) {
			this->setupServer(listener);
			printf("%d: Set up server\n", this->id_);
		} else {
			printf("%d: Ready to connect to network\n", this->id_);
			this->setupClient(listener);
			printf("%d: Connected to network\n", this->id_);
		}
		this->run();
	}

	virtual void setupServer(int listener) {
		this->pollfds_ = new Pollfds();
		this->addressBook_ = new NodeInfoArray();
		this->addressBook_->pushBack(this->myInfo_);
		this->pollfds_->pushBack(listener);
		printf("%d: Got necessary structs for server, and ready to accept\n", this->id_);
		this->acceptConnections(listener);
		this->broadcastAddressBook();
	}


	virtual void acceptConnections(int listener) {
		int expectedConnections = this->maxNodes_ - 1;
		printf("%d: Obligated to accept %d connections\n", this->id_, expectedConnections);
		for (int i = 0; i < expectedConnections; ++i) {
			NodeInfo* info = accept_connection(listener);
			printf("%d: Got a connection\n", this->id_);
			NodeInfo* previousInfo = this->addressBook_->find(info);
			if (previousInfo == nullptr) {
				this->addressBook_->pushBack(info);
				this->pollfds_->pushBack(info->get_fd());
			} else {
				int newFd = info->get_fd();
				previousInfo->set_fd(newFd);
				this->pollfds_->pushBack(newFd);
				delete info;
			}
		}
		printf("%d: Accepted all obligated connects\n", this->id_);
	}

	virtual void broadcastAddressBook() {
		printf("%d: Broadcasting address book\n", this->id_);
		Message* m = new Message(MsgKind::Directory);
		Serializer* s = new Serializer();
		s->clear();
		s->serialize_node_info_array(this->addressBook_);
		m->pack_body(s->get_buff(), s->get_size(), SerDesTypes::NODEINFOARRAY, this->addressBook_->len());
		for (size_t i = 0; i < this->addressBook_->len(); ++i) {
			send_message(this->addressBook_->get(i)->get_fd(), m);
		}
		delete m;
		delete s;
		printf("%d: Broadcasted address book\n", this->id_);
	}

	virtual void setupClient(int listener) {
		printf("%d: Connecting to server\n", this->id_);
		int serverFd = this->connectToServer(listener);
		printf("%d: Connected to server\n", this->id_);

		printf("%d: Getting address book\n", this->id_);
		this->getAddressBook(serverFd);
		printf("%d: Got address book\n", this->id_);

		// Comment this out if we want to debug
		int idx = this->addressBook_->indexOf(this->myInfo_);
		assert(idx != -1);
		this->id_ = idx;

		this->connectToLesserNodes();
		printf("%d: Connected to assigned nodes\n", this->id_);
		this->acceptConnections(listener);
		printf("%d: Accepted to assigned nodes\n", this->id_);
	}

	virtual int connectToServer(int listener) {
		NodeInfo* serverInfo = new NodeInfo(new String(SERVER_IP), SERVER_PORT);
		int serverFd = connect_to(this->myInfo_, serverInfo);
		serverInfo->set_fd(serverFd);
		this->pollfds_->pushBack(listener);
		this->pollfds_->pushBack(serverFd);
		return serverFd;
	}

	virtual void getAddressBook(int serverFd) {
		Message m = Message();
		read_message(serverFd, &m);
		assert (m.get_kind() == MsgKind::Directory);
		size_t n = m.get_ser_member_size();
		assert (n == this->maxNodes_);
		Deserializer d = Deserializer();
		this->addressBook_ = d.deserialize_node_info_array(m.get_body(), n);
	}

	virtual void connectToLesserNodes() {
		for (size_t i = 1; i < NUM_NODES; ++i) {
			if (i == this->id_) {
				break;
			}
			printf("%d: Connect to %lu\n", this->id_, i);
			NodeInfo* otherInfo = this->addressBook_->get(i);
			int fd = connect_to(this->myInfo_, otherInfo);
			otherInfo->set_fd(fd);
			this->pollfds_->pushBack(fd);
			printf("%d: Connected to %lu\n", this->id_, i);
		}
	}

	virtual void run() {
		sleep(5);

	}

	virtual void handleMessages() {}
};

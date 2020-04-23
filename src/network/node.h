#pragma once


#include "../util/object.h"
#include "../util/string.h"
#include "pollfds.h"
#include "node_info_array.h"
#include "network.h"
#include "node_configuration.h"

class Node : public Object {
public:
	size_t id_;
	NodeInfo* myInfo_; // owned
	NodeInfo* serverInfo_; // owned
	NodeInfoArray* addressBook_; // owned
	Pollfds* pollfds_; // owned
	bool running_;
	size_t numNodes_;
	int socketBacklog_;
	bool isServer_;

	Node(NodeConfiguration* conf) {
		this->myInfo_ = new NodeInfo(conf->getIP(), conf->getPort());
		this->serverInfo_ = new NodeInfo(conf->getServerIP(), conf->getServerPort());
		this->addressBook_ = nullptr;
		this->pollfds_ = new Pollfds();
		this->running_ = true;
		this->numNodes_ = conf->getNumNodes();
		this->socketBacklog_ = conf->getSocketBacklog();
		this->isServer_ = conf->getIsServer();
		this->initialize();
	}

	virtual ~Node() {
		delete this->myInfo_;
		delete this->serverInfo_;
		delete this->addressBook_;
		delete this->pollfds_;
		printf("%d: Deleted Node\n", this->myInfo_->getPort());
	}

	virtual void initialize() {
		printf("%d: Started Run\n", this->myInfo_->getPort());

		int listener = getNewFd();
		printf("%d: got fd\n", this->myInfo_->getPort());

		bindFdToAddress(listener, this->myInfo_);
		printf("%d: Binded fd\n", this->myInfo_->getPort());

		assert (listen(listener, this->socketBacklog_) == 0);
		this->myInfo_->setFd(listener);
		printf("%d: Got Listener\n", this->myInfo_->getPort());

		if (this->isServer_) {
			this->setupServer(listener);
			printf("%d: Set up server\n", this->myInfo_->getPort());
		} else {
			printf("%d: Ready to connect to network\n", this->myInfo_->getPort());
			this->setupClient(listener);
			printf("%d: Connected to network\n", this->myInfo_->getPort());
		}
	}

	virtual void setupServer(int listener) {
		this->id_ = 0;
		this->addressBook_ = new NodeInfoArray();
		this->addressBook_->pushBack(this->myInfo_);
		this->pollfds_->pushBack(listener);
		printf("%d: Got necessary structs for server, and ready to accept\n", this->myInfo_->getPort());
		this->acceptConnections(listener);
		if (this->numNodes_ > 1) {
			this->broadcastAddressBook();
		}
	}

	virtual void acceptConnections(int listener) {
		size_t expectedConnections = this->numNodes_ - (this->id_ + 1);
		printf("%d: Obligated to accept %lu connections\n", this->myInfo_->getPort(), expectedConnections);
		for (size_t i = 0; i < expectedConnections; ++i) {
			NodeInfo* info = acceptConnection(listener);
			printf("%d: %s:%d just connected\n", this->myInfo_->getPort(), info->getIP()->c_str(), info->getPort());
			NodeInfo* previousInfo = this->addressBook_->find(info);
			if (previousInfo == nullptr) {
				this->addressBook_->pushBack(info);
				this->pollfds_->pushBack(info->getFd());
			} else {
				int newFd = info->getFd();
				previousInfo->setFd(newFd);
				this->pollfds_->pushBack(newFd);
				delete info;
			}
		}
		printf("%d: Accepted all obligated connects\n", this->myInfo_->getPort());
	}

	virtual void broadcastAddressBook() {
		printf("%d: Broadcasting address book\n", this->myInfo_->getPort());
		String* body = this->addressBook_->serialize();
		Message m = Message(MsgKind::DIRECTORY, DataType::NODE_INFO_ARRAY, body);
		for (size_t i = 1; i < this->addressBook_->size(); ++i) {
			sendMessage(this->addressBook_->get(i)->getFd(), &m);
			printf("%d: Broadcasted address book to %lu\n", this->myInfo_->getPort(), i);
		}
		printf("%d: Broadcasted address book\n", this->myInfo_->getPort());
		delete body;
	}

	virtual void setupClient(int listener) {
		this->pollfds_->pushBack(listener);
		printf("%d: Connecting to server\n", this->myInfo_->getPort());
		int serverFd = this->connectToServer(listener);
		printf("%d: Connected to server\n", this->myInfo_->getPort());

		printf("%d: Getting address book\n", this->myInfo_->getPort());
		this->getAddressBook(serverFd);
		printf("%d: Got address book\n", this->myInfo_->getPort());
		this->addressBook_->find(this->serverInfo_)->setFd(serverFd);

		// Comment this out if we want to debug
		size_t idx = this->addressBook_->indexOf(this->myInfo_);
		assert(idx != SIZE_MAX);
		this->id_ = idx;

		this->connectToLesserNodes();
		printf("%d: Connected to assigned nodes\n", this->myInfo_->getPort());
		this->acceptConnections(listener);
		printf("%d: Accepted to assigned nodes\n", this->myInfo_->getPort());
	}

	virtual int connectToServer(int listener) {
		int serverFd = connectTo(this->myInfo_, this->serverInfo_);
		this->serverInfo_->setFd(serverFd);
		this->pollfds_->pushBack(serverFd);
		return serverFd;
	}

	virtual void getAddressBook(int serverFd) {
		Message* m = readMessage(serverFd);
		assert (m->getKind() == MsgKind::DIRECTORY);
		size_t counter = 0;
		this->addressBook_ = NodeInfoArray::deserialize(m->getBody(), &counter);
		assert(this->addressBook_->size() == this->numNodes_);
	}

	virtual void connectToLesserNodes() {
		for (size_t i = 1; i < this->numNodes_; ++i) {
			if (i == this->id_) {
				break;
			}
			printf("%d: Connect to %lu\n", this->myInfo_->getPort(), i);
			NodeInfo* otherInfo = this->addressBook_->get(i);
			int fd = connectTo(this->myInfo_, otherInfo);
			otherInfo->setFd(fd);
			this->pollfds_->pushBack(fd);
			printf("%d: Connected to %lu\n", this->myInfo_->getPort(), i);
		}
	}

	virtual size_t getNodeId() { return this->id_; }

	virtual void handleMessages() {}

	virtual void shutdown() { this->running_ = false; }
};
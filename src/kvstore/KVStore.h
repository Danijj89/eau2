#pragma once


#include <unordered_map>
#include <unistd.h>
#include "value.h"
#include "key.h"
#include "../network/node.h"
#include "../network/message.h"
#include "../util/lock.h"
#include "pair.h"
#include "../network/network.h"
#include "key_array.h"
#include "../network/node_configuration.h"

class KVStore : public Node {
public:
	std::unordered_map<Key, Value, KeyHashFunction> store_;
	KeyArray* userKeys_; // owned. Hold the keys that users create
	std::thread kvThread_;
	Lock* lock_; // owned
	Message* replyMessage_; // owned
	Key* cache_key_; // owned
	Value* cache_value_; // owned
	size_t chunkItems_;

	KVStore(NodeConfiguration* conf) : Node(conf) {
		this->store_ = std::unordered_map<Key, Value, KeyHashFunction>();
		this->userKeys_ = new KeyArray();
		this->kvThread_ = std::thread(&KVStore::handleMessages, this);
		this->lock_ = new Lock();
		this->replyMessage_ = nullptr;
		this->cache_key_ = nullptr;
		this->cache_value_ = nullptr;
		this->chunkItems_ = conf->getChunkItems();
	}

	~KVStore() {
		delete this->userKeys_;
		delete this->lock_;
		delete this->replyMessage_;
		delete this->cache_key_;
		delete this->cache_value_;
	}

	Value* get(Key* k) {
		printf("%lu: getting key %s stored in node %lu\n", this->id_, k->getKey()->c_str(), k->getNodeId());
		if (k->getNodeId() == this->id_) {
			return &(this->store_[*k]);
		}
		Value* cacheVal = this->getFromLocalCache(k);
		if (cacheVal != nullptr) return cacheVal;
		Value* v = this->requestValue(k);
		assert(v != nullptr);
		this->updateCache(k, v);
		return v;
	}

	Value* getFromLocalCache(Key* k) {
		if (k->equals(this->cache_key_)) return this->cache_value_;
		return nullptr;
	}

	Value* requestValue(Key* k) {
		printf("%lu: requesting value for key %s from node %lu\n", this->id_, k->getKey()->c_str(), k->getNodeId());
		Message m = Message(MsgKind::GET, DataType::KEY, k->serialize());

		// request value
		sendMessage(this->addressBook_->get(k->getNodeId())->getFd(), &m);

		// wait for value
		this->lock_->lock();
		while (this->replyMessage_ == nullptr) {
			this->lock_->wait();
		}
		size_t counter = 0;
		Value* v = Value::deserialize(this->replyMessage_->getBody(), this->replyMessage_->getSize(), &counter);
		// clean up read message
		delete this->replyMessage_;
		this->replyMessage_ = nullptr;
		this->lock_->unlock();
		return v;
	}

	void updateCache(Key* k, Value* v) {
		delete this->cache_key_;
		delete this->cache_value_;
		this->cache_key_ = new Key(k);
		this->cache_value_ = v;
	}

	/**
	 * Puts the given value associated with the given key in the store.
	 * This methods steals the key and the value.
	 * @param k
	 * @param v
	 */
	void put(Key* k, Value* v) {
		printf("%lu: put key %s in node %lu\n", this->id_, k->getKey()->c_str(), k->getNodeId());
		size_t nodeId = k->getNodeId();
		if (nodeId == this->id_) {
			this->store_[*k] = *v;
		}
		else {
			Pair pair = Pair(k, v);
			Message putMessage = Message(MsgKind::PUT, DataType::PAIR, pair.serialize());
			sendMessage(this->addressBook_->get(nodeId)->getFd(), &putMessage);
			delete k;
			delete v;
		}
	}

	/**
	 * Stores the user created value locally with given associated key, and broadcasts
	 * the key to the other nodes.
	 * Steals both key and value.
	 * @param k key
	 * @param v value
	 */
	void putUserData(Key* k, Value* v) {
		printf("%lu, put user key %s in node %lu\n", this->id_, k->getKey()->c_str(), this->id_);
		k->setNodeId(this->id_);
		this->store_[*k] = *v;
		this->userKeys_->pushBack(k);
		this->broadcastKey(k);
	}

	void broadcastKey(Key* k) {
		for (size_t i = 0; i < this->addressBook_->size(); i++) {
			if (i != this->id_) {
				Message addKeyMessage = Message(MsgKind::ADD_KEY, DataType::KEY, k->serialize());
				sendMessage(this->addressBook_->get(i)->getFd(), &addKeyMessage);
			}
		}
	}

	Value* getUserData(Key* k) {
		printf("%lu: get user data for key %s from node %lu\n", this->id_, k->getKey()->c_str(), k->getNodeId());
		Key* localKey = this->userKeys_->find(k);
		assert(localKey != nullptr);
		return this->get(localKey);
	}

	Value* waitAndGetUserData(Key* k) {
		printf("%lu: wait and get user data for key %s\n", this->id_, k->getKey()->c_str());
		Key* localKey;
		while (this->running_ && (localKey = this->userKeys_->find(k)) == nullptr) {
			sleep(2);
		}
		printf("%lu: got user data for key %s", this->id_, k->getKey()->c_str());
		return this->get(this->userKeys_->find(localKey));
	}

	void handleMessages() override {
		while(this->running_) {
			pollfd* fds = this->pollfds_->getPollfds();
			size_t len = this->pollfds_->size();
			if (poll(fds, len, -1) == -1) {
				fprintf(stderr, "Error with poll mechanism.\n");
			}
			for (size_t i = 0; i < len; ++i) {
				if (fds[i].revents == POLLIN) {
					int fd = fds[i].fd;
					Message* m = readMessage(fd);
					switch (m->getKind()) {
						case MsgKind::GET:
							printf("%lu: received GET message\n", this->id_);
							this->handleGetMessage(fd, m);
							delete m;
							break;
						case MsgKind::PUT:
							printf("%lu: received PUT message\n", this->id_);
							this->handlePutMessage(m);
							delete m;
							break;
						case MsgKind::ADD_KEY:
							printf("%lu: received ADD_KEY message\n", this->id_);
							this->handleAddKeyMessage(m);
							delete m;
							break;
						case MsgKind::REPLY:
							printf("%lu: received REPLY message\n", this->id_);
							this->handleReplyMessage(m);
							break;
						case MsgKind::SHUTDOWN:
							printf("%lu: received SHUTDOWN message\n", this->id_);
							delete m;
							this->running_ = false;
							break;
						default:
							assert(false);
					}
				}
			}
		}
		printf("%lu: stopped handling messages\n", this->id_);
	}

	void handleGetMessage(int fd, Message* m) {
		size_t counter = 0;
		Key* k = Key::deserialize(m->getBody(), &counter);
		Value* v = &this->store_[*k];
		printf("%lu: handling GET request for key %s\n", this->id_, k->getKey()->c_str());
		String* body = new String(v->getBlob(), v->getSize());
		Message replyMessage = Message(MsgKind::REPLY, DataType::VALUE, body);
		sendMessage(fd, &replyMessage);
		delete body;
		delete k;
	}

	void handlePutMessage(Message* m) {
		size_t counter = 0;
		Pair* pair = Pair::deserialize(m->getBody(), m->getSize(), &counter);
		Key* k = pair->getKey();
		k->setNodeId(this->id_);
		this->store_[*k] = *pair->getValue();
		delete pair;
	}

	void handleAddKeyMessage(Message* m) {
		size_t counter = 0;
		Key* k = Key::deserialize(m->getBody(), &counter);
		this->userKeys_->pushBack(k);
	}

	void handleReplyMessage(Message* m) {
		this->lock_->lock();
		this->replyMessage_ = m;
		this->lock_->notify_all();
		this->lock_->unlock();
	}

	void shutdown() override {
		String s("");
		Message m = Message(MsgKind::SHUTDOWN, DataType::EMPTY, &s);
		for (size_t i = 0; i < this->addressBook_->size(); i++) {
			if (i != this->id_) sendMessage(this->addressBook_->get(i)->getFd(), &m);
		}
		this->serverInfo_->setFd(-1);
		delete this->addressBook_;
		this->addressBook_ = nullptr;
		this->running_ = false;
	}

	size_t getChunkItems() { return this->chunkItems_; }
};
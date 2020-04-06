#pragma once


#include <unordered_map>
#include <sys/poll.h>
#include "key.h"
#include "value.h"
#include "key_array.h"
#include "value_array.h"
#include "../util/lock.h"
#include "../network/message.h"
#include "../network/kernode.h"




class KVStore : public Kernode {
public:
	int id_;
	std::unordered_map<Key, Value, KeyHashFunction> store_;
	Key* cache_key_;
	Value* cache_value_;
	Message* replyMessage_;
	Lock* lock_;
	size_t capacities_[NUM_NODES] = {0};
	std::thread* kvThread_;

	//TODO: remove id from kernode
	KVStore(int id, String* ip, int port) : Kernode(id, ip, port) {
		this->id_ = id;
		this->cache_key_ = nullptr;
		this->cache_value_ = nullptr;
		this->replyMessage_ = nullptr;
		this->lock_ = new Lock();
	}

	~KVStore() {
		delete this->cache_key_;
		delete this->cache_value_;
		delete this->replyMessage_;
		delete this->lock_;
	}

	Value* get(Key* k) {
		// check in cache
		Value* val = this->getFromCache(k);
		if (val != nullptr) return val;

		// check in local store
		assert(this->store_.find(k) != this->store_.end());
		Key localKey = this->store_.find(k)->first;
		int nodeId = localKey.getNodeId();
		if (nodeId == this->id_) return &this->store_[localKey];

		// check in entire store
		return this->requestValue(&localKey, nodeId);
	}

	Value* getFromCache(Key* k) {
		if (this->cache_key_->equals(k)) {
			return this->cache_value_;
		}
		return nullptr;
	}

	Value* requestValue(Key* k, int nodeId) {
		Serializer s = Serializer();
		s.serialize_key(k);
		Message m = Message(MsgKind::Get);
		m.pack_body(s.get_buff(), s.get_size(), SerDesTypes::KEY, 1);

		// request value
		send_message(this->addressBook_->get(nodeId)->get_fd(), &m);

		// wait for value
		this->lock_->lock();
		while (this->replyMessage_ == nullptr) {
			this->lock_->wait();
		}
		Deserializer d = Deserializer();
		Value* v = d.deserialize_value(this->replyMessage_->get_body());
		// clean up read message
		delete this->replyMessage_;
		this->replyMessage_ = nullptr;
		this->lock_->unlock();
		return v;
	}

	void put(Key* k, Value* v) {
		if (this->id_ == SERVER_ID) {
			int nodeId = this->getNodeWithCapacity();
			k->setNodeId(nodeId);
			if (nodeId == this->id_) {
				this->store_[*k] = *v;
			} else {
				Message putMessage = Message();
				send_message(this->addressBook_->get(nodeId)->get_fd(), &putMessage);
			}
			// broadcast the new key to every node in the network
			this->broadcastKey(k);
		} else {
			Serializer s = Serializer();
			s.serialize_key_value_pair(k, v);
			Message m = Message(MsgKind::Put);
			m.pack_body(s.get_buff(), s.get_size(), SerDesTypes::PAIR, 1);
			// send put request to server
			send_message(this->addressBook_->get(0)->get_fd(), &m);
		}
	}

	int getNodeWithCapacity() {
		for (int i = 0; i < NUM_NODES; i++) {
			if (this->capacities_[i] < MAX_BLOBS_PER_NODE) {
				this->capacities_[i]++;
				return i;
			}
		}
		// no space left in the entire network
		assert(false);
	}

	void broadcastKey(Key* k) {
		for (int i = 0; i < NUM_NODES; i++) {
			if (i != this->id_) {
				Message addKey = Message();
				addKey.pack_addkey_message(k);
				send_message(this->addressBook_->get(i)->get_fd(), &addKey);
			}
		}
	}

	Value* waitAndGet(Key* k) {
		while (this->running_ && this->store_.find(k) == this->store_.end()) {
			sleep(2);
		}
		return this->get(k);
	}

	//TODO: continue from here, need to handle the following messages:
	//	get, put, addkey, replyToGet
	//	note that all keys are being updated on all nodes
	void handleMessages() override {
		while(this->running_) {
			Message* buf = new Message();
			pollfd* fds = this->pollfds_->getPfds();
			size_t len = this->pollfds_->len();
			if (poll(fds, len, -1) == -1) {
				fprintf(stderr, "Error with poll mechanism.\n");
			}
			for (size_t i = 0; i < len; ++i) {
				if (fds[i].revents == POLLIN) {
					int fd = fds[i].fd;
					read_message(fd, buf);
					switch (buf->get_kind()) {
						case MsgKind::Get:
							this->handleGetMessage(fd, buf);
							delete buf;
							break;
						case MsgKind::Put:
							this->handlePutMessage(buf);
							delete buf;
							break;
						case MsgKind::AddKey:
							this->handleAddKeyMessage(buf);
							delete buf;
							break;
						case MsgKind::Reply:
							this->handleReplyMessage(buf);
							break;
						default:
							assert(false);
					}
				}
			}
		}
	}

	void handleGetMessage(int fd, Message* m) {
		Deserializer d = Deserializer();
		Key* k = d.deserialize_key(m->get_body());
		Value v = this->store_[*k];
		Message replyMessage = Message();
		replyMessage.pack_reply_message(&v);
		send_message(fd, &replyMessage);
		delete k;
	}

	void handlePutMessage(Message* m) {
		char* buff = m->get_body();
		Deserializer d = Deserializer();
		String* key = d.deserialize_string(buff);
		int nodeId = d.deserialize_int(buff + key->size() + 1);
		Value* v = d.deserialize_value(buff + key->size() + 5);
		Key k = Key(key, nodeId);
		this->store_[k] = *v;
		delete key;
		delete v;
	}

	void handleAddKeyMessage(Message* m) {
		Deserializer d = Deserializer();
		Key* k = d.deserialize_key(m->get_body());
		this->store_[*k] = Value();
	}

	void handleReplyMessage(Message* m) {
		this->lock_->lock();
		this->replyMessage_ = m;
		this->lock_->notify_all();
		this->lock_->unlock();
	}

	void run() override {
		this->kvThread_ = new std::thread(&KVStore::handleMessages, this);
	}
};

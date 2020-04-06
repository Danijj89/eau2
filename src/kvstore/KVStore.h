#pragma once

#include "key.h"
#include "value.h"
#include "../network/node.h"
#include "key_array.h"
#include "value_array.h"
#include "../network/kernode.h"
#include "../util/lock.h"
#include <unordered_map>

class KVStore : public Kernode {
public:
	int id_;
	std::unordered_map<Key, Value, KeyHashFunction> store;
	Key* cache_key_;
	Value* cache_value_;
	Message* replyMessage_;
	Lock* lock_;
	size_t capacities_[NUM_NODES] = {0};

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
		assert(this->store.find(k) != this->store.end());
		Key localKey = this->store.find(k)->first;
		int nodeId = localKey.getNodeId();
		if (nodeId == this->id_) return &this->store[localKey];

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
				this->store[*k] = *v;
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
		while (this->store.find(k) == this->store.end()) {
			sleep(2);
		}
		return this->get(k);
	}
};

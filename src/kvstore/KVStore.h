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
	KeyArray* keys_;
	ValueArray* values_;
	Key* cache_key_;
	Value* cache_value_;
	Message* m = nullptr;
	Lock lock;

	KVStore(int id, String* ip, int port) : Kernode(id, ip, port) {
		this->id_ = id;
		this->keys_ = new KeyArray();
		this->values_ = new ValueArray();
		this->cache_key_ = nullptr;
		this->cache_value_ = nullptr;
	}

	~KVStore() {
		delete this->keys_;
		delete this->values_;
		delete this->cache_key_;
		delete this->cache_value_;
	}

	Value* get(Key* k) {
		int nodeId = k->getNodeId();
		// node id == -1 only if key is created by user
		if (nodeId == -1) {
			// search in cache
			Value* cacheVal = this->getFromCache(k);
			if (cacheVal != nullptr) return cacheVal;
			// search in store
			size_t idx = this->keys_->indexOf(k);
			assert(idx != SIZE_MAX);
			nodeId = this->keys_->get(idx)->getNodeId();
			if (nodeId == this->id_) return this->values_->get(idx);
		}
		else if (nodeId == this->id_) {
			return this->getFromLocal(k);
		}
		return this->requestValue(k, nodeId);
	}

	Value* getFromLocal(Key* k) {
		size_t idx = this->keys_->indexOf(k);
		assert(idx != SIZE_MAX);
		return this->values_->get(idx);
	}

	Value* requestValue(Key* k, int nodeId) {
		Serializer s = Serializer();
		s.serialize_key(k);
		Message m = Message(MsgKind::Get);
		m.pack_body(s.get_buff(), s.get_size(), SerDesTypes::KEY, 1);
		send_message(this->addressBook_->get(nodeId)->get_fd(), &m);
		this->lock.lock();
		while (this->m == nullptr) {
			this->lock.wait();
		}
		//
		//TODO: wait for response
	}

	int put(Key* k, Value* v) {
		if (this->id_ == 0) {

		} else {
			Message m = Message(MsgKind::Put);
			send_message(this->addressBook_->get(0), );
		}
		if (this->hasCapacity()) {
			k->setNodeId(this->id_);
			this->keys_->pushBack(new Key(k));
			this->values_->pushBack(v);
			return this->id_;
		}
		for (size_t i = 0; i < this->numStores_; i++) {
			if (this->stores_[i]->hasCapacity()) {
				return this->stores_[i]->put(k, v);
			}
		}
		this->broadcast();
		// no capacity in the entire network
		assert(false);
	}

	bool contains(Key* k) {
		size_t i = this->keys_->indexOf(k);
		return i != SIZE_MAX;
	}

	bool hasCapacity() {
		return this->keys_->len() < MAX_CHUNKS_PER_NODE;
	}

	void setStores(KVStore** stores, size_t n) {
		this->stores_ = stores;
		this->numStores_ = n;
	}

	Value* getFromCache(Key* k) {
		if (this->cache_key_->equals(k)) {
			return this->cache_value_;
		}
		return nullptr;
	}


//	Value* waitAndGet(Key* k) {
//		while (this->running_) {
//			Value* v = this->requestValue(k);
//			if (v != nullptr) {
//				return v;
//			}
//		}
//	}

//	Value* requestValue(Key* k) {
//		int fd = this->node_infos_->get(k->getNodeId())->get_fd();
//		Message m = Message(MsgKind::Get);
//		Serializer s = Serializer();
//		s.serialize_key(k);
//		write_message(fd, &m);
//	}

//	void joinKVS() {
//		size_t n = this->node_infos_->len();
//		for (size_t i = 0; i < n; ++i) {
//			this->connect_to_node(this->info_, this->node_infos_->get(i));
//		}
//	}

//	void broadcastKey(Key* k) {
//		Message* m = new Message(MsgKind::KEY);
//		Serializer* s = new Serializer();
//		s->serialize_key(k);
//		m->back_body(s->get_buff(), s->get_size(), SerDesTypes::KEY, 1);
//		size_t n = this->node_infos_->len();
//		for (size_t i = 0; i < n; ++i) {
//			send_message(this->node_infos_->get(i)->get_fd(), m);
//		}
//		delete m;
//		delete s;
//	}
//
//	//Need to add handle key broadcast
//	// e.g.
//	void handle_messages() {
//		...
//		switch(m.type) {
//		...
//		case MsgKind::KEY:
//			// Get the key from the serialization
//			this->keys_.pushBack(key);
//			this->values_.pushBack(nullptr);
//			break;
//		...
//		}
//		...
//	}
};

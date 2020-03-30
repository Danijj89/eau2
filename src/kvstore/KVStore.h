#pragma once

#include "key.h"
#include "value.h"
#include "../network/node.h"
#include "key_array.h"
#include "value_array.h"

class KVStore {
public:
	int id_;
	KeyArray* keys_;
	ValueArray* values_;
	Key* cache_key_;
	Value* cache_value_;
	KVStore** stores_;
	size_t numStores_;

	KVStore(int id) {
		this->id_ = id;
		this->keys_ = new KeyArray();
		this->values_ = new ValueArray();
		this->cache_key_ = nullptr;
		this->cache_value_ = nullptr;
		this->stores_ = nullptr;
		this->numStores_ = 0;
	}

	~KVStore() {
		delete this->keys_;
		delete this->values_;
		delete this->cache_key_;
		delete this->cache_value_;
	}

	Value* get(Key* k) {
		int nodeId = k->getNodeId();
		if (nodeId == -1) {
			size_t idx = this->keys_->indexOf(k);
			if (idx != SIZE_MAX) {
				return this->values_->get(idx);
			} else {
				for (size_t i = 0; i < this->numStores_; i++) {
					if (this->stores_[i]->contains(k)) {
						return this->stores_[i]->get(k);
					}
				}
				return nullptr;
			}
		}
		else if (nodeId == this->id_) {
			size_t i = this->keys_->indexOf(k);
			if (i == SIZE_MAX) return nullptr;
			return this->values_->get(i);
		}
		assert((size_t)nodeId < this->numStores_);
		return this->stores_[nodeId]->get(k);
	}

	int put(Key* k, Value* v) {
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

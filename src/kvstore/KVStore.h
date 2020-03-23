#pragma once

#include "key.h"
#include "value.h"
#include "../network/node.h"
#include "key_array.h"
#include "value_array.h"

class KVStore : public Node {
public:
	int id_;
	KeyArray* keys_;
	ValueArray* values_;
	Key* cache_key_;
	Value* cache_value_;

	KVStore(int id, String* ip, int port) : Node(ip, port) {
		this->id_ = id;
		this->keys_ = new KeyArray();
		this->values_ = new ValueArray();
		this->cache_key_ = nullptr;
		this->cache_value_ = nullptr;
	}

	Value* get(Key* k) {
		if (k->getNodeId() == this->id_) {
			size_t i = this->keys_->indexOf(k);
			if (i == -1) return nullptr;
			return this->values_->get(i);
		} else {
			// Won't come here until it is distribuited
			// return this->requestValue(k);
		}
	}

	void put(Key* k, Value* v) {
		if (k->getNodeId() < 0) {
			k->setNodeId(this->id_);
		}
		if (k->getNodeId() == this->id_) {
			this->keys_->pushBack(k);
			this->values_->pushBack(v);
			// this->broadcastKey(k);
		} else {
			//
//			Pair p = make_pair(k, v);
//			int fd = this->node_infos_->get(k->getKey())->get_fd();
//			send_pair(fd, p);
		}
	}

	Value* waitAndGet(Key* k) {
		while (this->running_) {
			Value* v = this->requestValue(k);
			if (v != nullptr) {
				return v;
			}
		}
	}

	Value* requestValue(Key* k) {
//		int fd = this->node_infos_->get(k->getNodeId())->get_fd();
//		Message m = Message(MsgKind::Get);
//		Serializer s = Serializer();
//		s.serialize_key(k);
//		write_message(fd, &m);
	}

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

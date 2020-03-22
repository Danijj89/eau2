#pragma once

#include <vector>
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

	Value* get(Key* k) {
		if (k->getNodeId() == this->id_) {
			size_t i = this->keys_->indexOf(k);
			if (i == -1) return nullptr;
			return this->values_->get(i);
		} else {
			return this->requestValue(k, fd);
		}
	}

	void put(Key* k, Value* v) {

	}

	Value* requestValue(Key* k) {
		int fd = this->node_infos_->get(k->getNodeId())->get_fd();
		Message m = Message(MsgKind::Get);
		Serializer s = Serializer();
		s.serialize_key(k);
		write_message(fd, &m);
	}


};
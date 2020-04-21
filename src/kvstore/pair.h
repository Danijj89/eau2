#pragma once

#include "../util/object.h"
#include "../util/string.h"
#include "key.h"
#include "value.h"


class Pair : public Object {
public:
	Key* key_; // owned
	Value* value_; // owned

	Pair(Key* k, Value* v) : key_(k), value_(v) {}

	Key* getKey() { return this->key_; }

	Value* getValue() { return this->value_; }

	String* serialize() {
		StrBuff sb = StrBuff();
		sb.c(*this->key_->serialize());
		sb.c(*this->value_->serialize());
		return sb.get();
	}

	static Pair* deserialize(char* buff, size_t size, size_t* counter) {
		Key* k = Key::deserialize(buff, counter);
		Value* v = Value::deserialize(buff, size, counter);
		return new Pair(k, v);
	}
};
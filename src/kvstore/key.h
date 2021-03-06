#pragma once


#include "../util/object.h"
#include "../util/string.h"
#include "value.h"
#include "../serialization/serializer.h"
#include "../serialization/deserializer.h"

/**
 * Key: This class represent the key in a key-value store.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class Key : public Object {
public:
	String* key_; //Owned
	size_t nodeId_ = SIZE_MAX;


	Key(const char* key) {
		this->key_ = new String(key);
	}

	Key(char* key) {
		this->key_ = new String(key);
	}

	Key(const char* key, int nodeId) : Key(new String(key)) {
		// this->key_ = new String(key);
		this->nodeId_ = nodeId;
	}

	// This constructor steals key.
	Key(String* key) {
		this->key_ = key;
	}

	Key(String* key, int nodeId) : Key(key) {
		this->nodeId_ = nodeId;
	}

	Key(Key* k) {
		size_t len = k->getKey()->size();
		char* str = new char[len + 1];
		memcpy(str, k->getKey()->c_str(), len);
		str[len] = '\0';
		this->key_ = new String(true, str, len);
		this->nodeId_ = k->getNodeId();
	}

	~Key() {
		delete this->key_;
	}

	String* getKey() {
		return this->key_;
	}

	size_t getNodeId() {
		return this->nodeId_;
	}

	void setNodeId(int nodeId) {
		this->nodeId_ = nodeId;
	}

	bool equals(Object* other) {
		Key* o= dynamic_cast<Key*>(other);
		if (other == nullptr) {
			return false;
		}
		return this->key_->equals(o->getKey());
	}

	bool operator==(const Key& k) const {
		return this->key_->equals(const_cast<Key*>(&k)->getKey());
	}

	String* serialize() {
		Serializer s = Serializer();
		s.serializeString(this->key_);
		s.serializeSizeT(this->nodeId_);
		return s.get();
	}

	static Key* deserialize(char* buff, size_t* counter) {
		Deserializer d = Deserializer();
		String* key = d.deserializeString(buff, counter);
		size_t nodeId = d.deserializeSizeT(buff, counter);
		return new Key(key, nodeId);
	}
};

class KeyHashFunction {
public:

	size_t operator()(const Key& k) const {
		size_t result = 0;
		String* s = const_cast<Key*>(&k)->getKey();
		char* val = s->c_str();
		for (size_t i = 0; i < s->size(); i++) {
			result += val[i];
		}
		return result;
	}
};

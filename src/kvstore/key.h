#pragma once


#include "../util/string.h"


/**
 * Key: This class represent the key in a key-value store.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class Key : public Object {
public:
	String* key_; //Owned
	int nodeId_ = -1;

	Key(const char* key) {
		this->key_ = new String(key);	
	}
	
	Key(char* key) {
		this->key_ = new String(key);
	}

	Key(const char* key, int nodeId) {
		this->key_ = new String(key);
		this->nodeId_ = nodeId;
	}

	// This constructor steals key.
	Key(String* key) {
		this->key_ = key;	
	}

	Key(String* key, int nodeId) : Key(key) {
		this->nodeId_ = nodeId;	
	}

	~Key() {
		delete this->key_;
	}

	String* getKey() {
		return this->key_;
	}

	void setNodeId(int nodeId) {
		this->nodeId_ = nodeId;
	}

	int getNodeId() {
		return this->nodeId_;
	}

	bool equals(Object* other) {
		Key* o= dynamic_cast<Key*>(other);
		if (other == nullptr) {
			return false;
		}
		return this->key_->equals(o->getKey());
	}
};

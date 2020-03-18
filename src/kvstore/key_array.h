#pragma once


#include "key.h"


/**
 * KeyArray: this class represent a vector of Key objects.
 * New keys can only be appended, and no key can be deleted.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class KeyArray : public Object {
public:
	Key** array_; //Owned
	size_t size_;
	size_t capacity_;

	KeyArray(size_t capacity) {
		this->array_ = new Key*[capacity];
		this->capacity_ = capacity;
		this->size_ = 0;
	}

	KeyArray() : KeyArray(128) {}

	~KeyArray() {
		for (size_t i = 0; i < this->size_; ++i) {
			delete this->array_[i];
		}
		delete[] this->array_;
	}

	// This method steals. Assumes unique Key objects.
	void pushBack(Key* key) {
		if (this->size_ == this->capacity_) {
			this->capacity_ *= 2;
			Key** new_array = new Key*[this->capacity_];
			for (size_t i = 0; i < this->size_; ++i) {
				new_array[i] = this->array_[i];
			}
			delete[] this->array_;
			this->array_ = new_array;
		}
		this->array_[this->size_] = key;
	}

	Key* getKey(size_t index) {
		if (index >= this->size_) {
			return nullptr;
		}
		return this->array_[index];
	}

	size_t getIndex(Key* key) {
		for (size_t i = 0; i < this->size_; ++i) {
			if (this->array_[i]->equals(key)) {
				return i;
			}
		}
		return SIZE_MAX;
	}
};

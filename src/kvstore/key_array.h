#pragma once


#include "../util/object.h"
#include "../util/array.h"
#include "key.h"

/**
 * KeyArray: this class represent a vector of Key objects.
 * New keys can only be appended, and no key can be deleted.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class KeyArray : public Object {
public:
	Array* array_; //Owned

	/**
	 * Default constructor of this array.
	 */
	KeyArray() {
		this->array_ = new Array();
	}

	/**
	 * Constructs a key array with the given initial capacity
	 * @param size
	 */
	KeyArray(size_t size) {
		this->array_ = new Array(size);
	}

	/**
	 * The destructor of this array.
	 */
	~KeyArray() {
		delete this->array_;
	}

	/**
	 * Pushes the given item to the end of this array.
	 *
	 * @param item the given item to be added to the end of this array
	 */
	void pushBack(Key* item) {
		this->array_->pushBack(item);
	}

	/**
	 * Set the element of the array at the given index
	 * with the given element.
	 * Throws an error if index is out of bounds.
	 *
	 * @param i insertion index
	 * @param v the element to be inserted at the given index
	 */
	void set(size_t i, Key* v) {
		this->array_->set(i, v);
	}

	/**
	 * Returns an element at the given index.
	 * Throws an error if index is out of bounds.
	 *
	 * @param i the index of the element
	 * @return the element of the array at the given index
	 */
	Key* get(size_t i) {
		Key* k = dynamic_cast<Key*>(this->array_->get(i));
		assert(k != nullptr);
		return k;
	}

	/**
	 * Finds the key in this array that is equal to the given key.
	 * @param k the key to find
	 * @return the key in this array or nullptr if not found
	 */
	Key* find(Key* k) {
		return dynamic_cast<Key*>(this->array_->find(k));
	}

	/**
	 * Gets the size of this array
	 * @return size
	 */
	size_t size() { return this->array_->size(); }

	String* serialize() {
		Serializer s = Serializer();
		StrBuff sb = StrBuff();
		sb.c(*s.serializeSizeT(this->size())->get());
		for (size_t i = 0; i < this->size(); i++) {
			Key* curr = dynamic_cast<Key*>(this->array_->get(i));
			assert(curr != nullptr);
			sb.c(*curr->serialize());
		}
		return sb.get();
	}

	static KeyArray* deserialize(char* buff, size_t* counter) {
		Deserializer d = Deserializer();
		size_t size = d.deserializeSizeT(buff, counter);
		KeyArray* result = new KeyArray(size);
		for (size_t i = 0; i < size; i++) {
			result->pushBack(Key::deserialize(buff, counter));
		}
		return result;
	}
};

#pragma once


#include "key.h"
#include "../util/array.h"


/**
 * KeyArray: this class represent a vector of Key objects.
 * New keys can only be appended, and no key can be deleted.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class KeyArray {
public:
	Array* array_; //Owned

	/**
	 * Default constructor of this array.
	 */
	KeyArray() {
		this->array_ = new Array();
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
	virtual void set(size_t i, Key* v) {
		this->array_->set(i, v);
	}

	/**
	 * Returns an element at the given index.
	 * Throws an error if index is out of bounds.
	 *
	 * @param i the index of the element
	 * @return the element of the array at the given index
	 */
	virtual Key* get(size_t i) {
		Key* k = dynamic_cast<Key*>(this->array_->get(i));
		assert(k != nullptr);
		return k;
	}

	/**
	 * Finds and returns the index of the given Key.
	 *
	 * @param k the key to find
	 * @return the index or SIZE_MAX if not found
	 */
	size_t indexOf(Key* k) {
		for (size_t i = 0; i < this->array_->size(); ++i) {
			Key* curr = this->get(i);
			if (curr->equals(k)) return i;
		}
		return SIZE_MAX;
	}

	/**
	 * Gets the size of this array
	 * @return size
	 */
	size_t size() { return this->array_->size(); }
};

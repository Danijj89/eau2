#pragma once


#include "key.h"
#include "../util/array.h"


/**
 * KeyArray: this class represent a vector of Key objects.
 * New keys can only be appended, and no key can be deleted.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class KeyArray : public Array {
public:
	Key** vals_; //Owned

	/**
	 * Default constructor of this array.
	 */
	KeyArray() : Array() {
		this->vals_ = new Key*[this->capacity_];
	}

	/**
	 * The destructor of this array.
	 */
	~KeyArray() {
		for (size_t i = 0; i < this->size_; ++i) {
			delete this->vals_[i];
		}
		delete[] this->vals_;
	}

	/**
	 * Pushes the given item to the end of this array.
	 *
	 * @param item the given item to be added to the end of this array
	 */
	virtual void pushBack(Key* item) {
		this->resizeIfFull();
		this->vals_[this->size_] = item;
		this->size_ += 1;
	}

	/**
	 * Grows the array if it has reached it's capacity limit.
	 */
	void resize() override {
		this->capacity_ *= 2;
		Key** new_vals = new Key*[this->capacity_];
		for (size_t i = 0; i < this->size_; ++i) {
			new_vals[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = new_vals;
	}

	/**
	 * Set the element of the array at the given index
	 * with the given element.
	 * Throws an error if index is out of bounds.
	 *
	 * @param i insertion index
	 * @param element the element to be inserted at the given index
	 */
	virtual void set(size_t i, Key* element) {
		this->assertIndexInBounds(i);
		this->vals_[i] = element;
	}

	/**
	 * Returns an element at the given index.
	 * Throws an error if index is out of bounds.
	 *
	 * @param i the index of the element
	 * @return the element of the array at the given index
	 */
	virtual Key* get(size_t i) {
		assert(i < this->size_);
		if (i >= this->size_) {
			return nullptr;
		}
		return this->vals_[i];
	}

	/**
	 * Finds and returns the index of the given Key.
	 *
	 * @param k the key to find
	 * @return the index or SIZE_MAX if not found
	 */
	size_t indexOf(Key* k) {
		for (size_t i = 0; i < this->size_; ++i) {
			if (this->vals_[i]->equals(k)) {
				return i;
			}
		}
		return SIZE_MAX;
	}
};

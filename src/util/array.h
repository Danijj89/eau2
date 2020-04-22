//lang:CwC


#pragma once


#include <cstdio>
#include <cassert>
#include "object.h"


/**
 * A generic array of objects. All objects put into the array are
 * stolen.
 *
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */
class Array : public Object {
public:
	Object** vals_; // Owned
	size_t size_; // num of elements
	size_t capacity_; // capacity of array

	/**
	 * Default constructor.
	 */
	Array() {
		this->size_ = 0;
		this->capacity_ = 4;
		this->vals_ = new Object*[this->capacity_];
	}

	/**
	 * Constructs an array of the given size.
	 * @param size
	 */
	Array(size_t size) {
		this->size_ = 0;
		this->capacity_ = size;
		this->vals_ = new Object*[this->capacity_];
	}

	/**
	 * Default destructor.
	 */
	~Array() {
		for (size_t i = 0; i < this->size_; i++) {
			delete this->vals_[i];
		}
		delete[] this->vals_;
	}

	/**
	 * Returns the size of this array.
	 * @return the length.
	 */
	size_t size() {
		return this->size_;
	}

	/**
	 * Sets the given value at the given index.
	 * @param i idx
	 * @param v value
	 */
	void set(size_t i, Object* v) {
		assert(i < this->size_);
		delete this->vals_[i];
		this->vals_[i] = v;
	}

	/**
	 * Gets the object at the given index.
	 * @param i idx
	 * @return object
	 */
	Object* get(size_t i) {
		assert(i < this->size_);
		return this->vals_[i];
	}

	/**
	 * Adds the given value at the end of this array.
	 * @param v object
	 */
	void pushBack(Object* v) {
		if (this->size_ == this->capacity_) this->resize_();
		this->vals_[this->size_] = v;
		this->size_++;
	}

	/**
	 * Resizes the array.
	 */
	void resize_() {
		this->capacity_ *= 2;
		Object** temp = new Object*[this->capacity_];
		for (size_t i = 0; i < this->size_; i++) {
			temp[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = temp;
	}

	/**
	 * Returns the index of the first object that equals the given one
	 * in this array, or SIZE_MAX if the object could not be found.
	 *
	 * @param o the object to find
	 * @return the index or SIZE_MAX if not found
	 */
	size_t indexOf(Object* o) {
		for (size_t i = 0; i < this->size_; ++i) {
			if (o->equals(this->vals_[i])) return i;
		}
		return SIZE_MAX;
	}

	/**
	 * Returns the pointer to object in this array that is equal to the
	 * given one, or nullptr if the equivalent object does not exist.
	 *
	 * @param o the object to find
	 * @return the object in this array or nullptr if not found
	 */
	Object* find(Object* o) {
		size_t i = this->indexOf(o);
		if (i != SIZE_MAX) return this->vals_[i];
		return nullptr;
	}
};
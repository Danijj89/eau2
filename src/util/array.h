//lang:CwC


#pragma once


#include <cstdio>
#include <cassert>
#include "object.h"


/**
 * A generic array of objects
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */
class Array : public Object {
public:
	Object** vals_;
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
		Object** temp = new Object*[this->capacity_ * 2];
		for (size_t i = 0; i < this->size_; i++) {
			temp[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = temp;
	}
};
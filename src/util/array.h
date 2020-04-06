//lang:CwC


#pragma once


#include <cstdio>
#include <cassert>
#include "object.h"


/**
 * A superclass for resizable arrays.
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */
class Array : Object {
public:
	size_t size_; // num of elements
	size_t capacity_; // capacity of array

	/**
	 * Default constructor.
	 */
	Array() {
		this->size_ = 0;
		this->capacity_ = 4;
	}

	/**
	 * Returns the length of this array.
	 * @return the length.
	 */
	virtual size_t len() {
		return this->size_;
	}

	/**
	 * Asserts that the given index is not out of bounds.
	 * @param idx the index.
	 */
	virtual void assertIndexInBounds(size_t idx) {
		assert(idx >= 0 && idx < this->size_);
	}

	/**
     * Empties this array of all its values.
     */
	virtual void clear() {
		this->size_ = 0;
	}

	/**
	 * Resize the array if it has reached its full capacity.
	 */
	virtual void resizeIfFull() {
		if (this->size_ >= this->capacity_) {
			this->resize();
		}
	}

	/**
	 * Actual resize method.
	 * Need to be overridden by subclasses that uses this feature.
	 */
	virtual void resize() {}

	/**
	 * Removes the item at given index.
	 */
	virtual void remove(size_t idx) {}
};
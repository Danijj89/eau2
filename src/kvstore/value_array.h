#pragma once


#include "value.h"
#include "../util/array.h"


/**
 * ValueArray: this class represent a vector of Value objects.
 * New Values can only be appended, and no Value can be deleted.
 * Authors: cao.yuan1@husky.neu.edu, zhan.d@husky.neu.edu
 */
class ValueArray : public Array {
public:
	Value** vals_; //Owned

	/**
	 * Default constructor of this array.
	 */
	ValueArray() : Array() {
		this->vals_ = new Value*[this->capacity_];
	}

	/**
	 * The destructor of this array.
	 */
	~ValueArray() override {
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
	virtual void pushBack(Value* item) {
		this->resizeIfFull();
		this->vals_[this->size_] = item;
		this->size_ += 1;
	}

	/**
	 * Grows the array if it has reached it's capacity limit.
	 */
	void resize() override {
		this->capacity_ *= 2;
		Value** new_vals = new Value*[this->capacity_];
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
	virtual void set(size_t i, Value* element) {
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
	virtual Value* get(size_t i) {
		assert(i < this->size_);
		if (i >= this->size_) {
			return nullptr;
		}
		return this->vals_[i];
	}

	/**
	 * Finds and returns the index of the given Value.
	 *
	 * @param v the Value to find
	 * @return the index or SIZE_MAX if not found
	 */
	size_t indexOf(Value* v) {
		for (size_t i = 0; i < this->size_; ++i) {
			if (this->vals_[i]->equals(v)) {
				return i;
			}
		}
		return SIZE_MAX;
	}
};

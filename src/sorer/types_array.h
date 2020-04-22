//lang::CwC


#pragma once


#include "types.h"
#include <cstdlib>
#include <cassert>

/**
 * Resizable array of types.
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */
class TypesArray {
public:
    Types* vals_; // the values (owned)
    size_t size_;
    size_t capacity_;

    /**
     * Default constructor of this array.
     */
    TypesArray() {
    	this->size_ = 0;
    	this->capacity_ = 4;
        this->vals_ = new Types[this->capacity_];
    }

    /**
     * The destructor of this array.
     */
    ~TypesArray() {
        delete[] this->vals_;
    }

    /**
     * Grows the array if it has reached it's capacity limit.
     */
    void resize_() {
        this->capacity_ *= 2;
        Types* new_vals = new Types[this->capacity_];
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
    virtual void set(size_t i, Types element) {
        assert(i < this->size_);
        this->vals_[i] = element;
    }


    /**
     * Returns an element at the given index.
     * Throws an error if index is out of bounds.
     *
     * @param i the index of the element
     * @return the element of the array at the given index
     */
    virtual Types get(size_t i) {
        assert(i < this->size_);
        return this->vals_[i];
    }

    /**
     * Pushes the given item to the end of this array.
     *
     * @param item the given item to be added to the end of this array
     */
    virtual void pushBack(Types item) {
        if (this->size_ == this->capacity_) this->resize_();
        this->vals_[this->size_] = item;
        this->size_ += 1;
    }

    size_t size() { return this->size_; }

    void clear() { this->size_ = 0; }
};

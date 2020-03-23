#pragma once


#include "string.h"
#include "array.h"

/**
 * String array takes ownership of all the values that get added.
 */
class StringArray : public Array {
public:
    String** vals_; // owned

    /**
     * Default constructor.
     */
    StringArray() {
        this->vals_ = new String*[this->capacity_];
    }

    /**
     * Constructs an array of given capacity.
     * @param n the given capacity.
     */
    StringArray(size_t n) {
        this->size_ = 0;
        this->capacity_ = n;
        this->vals_ = new String*[this->capacity_];
    }

    /**
     * Default destructor.
     */
    ~StringArray() {
        for (size_t i = 0; i < this->size_; ++i) {
            delete this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Get the string at index i.
     * @param i the index.
     * @return the string.
     */
    String* get(size_t i) {
    	this->assertIndexInBounds(i);
        return this->vals_[i];
    }

    /**
     * Sets the value at index i with val.
     *
     * @param i the index.
     * @param val the value.
     */
    void set(size_t i, String* val) {
    	this->assertIndexInBounds(i);
        this->vals_[i] = val;
    }

    /**
     * Grows the array if it has reached it's capacity limit.
     */
    void resize() override {
        this->capacity_ *= 2;
        String** new_vals = new String*[this->capacity_];
        for (size_t i = 0; i < this->size_; ++i) {
            new_vals[i] = this->vals_[i];
        }
        delete[] this->vals_;
        this->vals_ = new_vals;
    }

    /**
     * Pushes the given item to the end of this array.
     * NOTE: nullptr are valid values
     *
     * @param item the given item to be added to the end of this array
     */
    virtual void pushBack(String* s) {
        this->resizeIfFull();
        this->vals_[this->size_] = s;
        this->size_++;
    }
};
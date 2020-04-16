#pragma once

#include "string.h"
#include "array.h"

/**
 * Array of strings.
 */
class StringArray : public Object {
public:
    Array* array_;

    /**
     * Default constructor.
     */
    StringArray() {
        this->array_ = new Array();
    }

    /**
     * Default destructor.
     */
    ~StringArray() {
        delete this->array_;
    }

    /**
     * Get the string at index i.
     * @param i the index.
     * @return the string.
     */
    String* get(size_t i) {
    	String* result = dynamic_cast<String*>(this->array_->get(i));
    	assert(result != nullptr);
    	return result;
    }

    /**
     * Sets the value at index i with val.
     *
     * @param i the index.
     * @param val the value.
     */
    void set(size_t i, String* val) {
    	this->array_->set(i, val);
    }

    /**
     * Pushes the given item to the end of this array.
     * NOTE: nullptr are valid values
     *
     * @param item the given item to be added to the end of this array
     */
     void pushBack(String* s) {
        this->array_->pushBack(s);
    }

    /**
     * Returns the size of this array.
     * @return size
     */
    size_t size() { return this->array_->size(); }
};
//lang::CwC


#pragma once

#include <cstdlib>
#include <cassert>

/**
 * FieldArray: represents a vector storing the starting and ending bytes of
 * fields in a .sor file relative to the start of the file.
 * The start of each field is delimited by '<' and end point by '>'.
 * The class also stores the type of all the delimited fields.
 * INVARIANT: the size of the starts and ends arrays are always the same
 * and the items stored at the respective indexes are referring to the
 * start and end of the same field.
 *
 * @authors cao.yuan1@husky.neu.edu & zhan.d@husky.neu.edu
 */
class FieldArray {
public:
    int* start_;
    int* end_;
    size_t size_;
    size_t capacity_;

    /**
     * Default constructor.
     */
    FieldArray() {
    	this->size_ = 0;
    	this->capacity_ = 4;
    	this->start_ = new int[this->capacity_];
    	this->end_ = new int[this->capacity_];
    }

    /**
     * Default destructor.
     */
    ~FieldArray() {
        delete[] this->start_;
        delete[] this->end_;
    }

    /**
     * Adds a new pair of start and end byte location of a field.
     * @param start the start byte
     * @param end the end byte
     */
    virtual void pushBack(int start, int end) {
        if (this->size_ == this->capacity_) this->resize_();
        this->start_[this->size_] = start;
        this->end_[this->size_] = end;
        this->size_ += 1;
    }

    /**
     * Grows the array if it has reached it's capacity limit.
     */
    void resize_() {
        this->capacity_ *= 2;
        int* new_start = new int[this->capacity_];
        int* new_end = new int[this->capacity_];
        for(size_t i = 0; i < this->size_; ++i) {
            new_start[i] = this->start_[i];
            new_end[i] = this->end_[i];
        }
        delete[] this->start_;
        delete[] this->end_;
        this->start_ = new_start;
        this->end_ = new_end;
    }

    /**
     * Returns the starting byte of the field at index i.
     * @param i the index of the field in this field array.
     * @return the starting byte, or -1 if the given index is out of bound.
     */
    virtual int getStart(size_t i) {
        assert(i < this->size_);
        return this->start_[i];
    }

    /**
     * Returns the ending byte of the field at index i.
     * @param i the index of the field in this field array.
     * @return the ending byte, or -1 if the given index is out of bound.
     */
    virtual int getEnd(size_t i) {
        assert(i < this->size_);
        return this->end_[i];
    }

    size_t size() { return this->size_; }

    void clear() { this->size_ = 0; }
};
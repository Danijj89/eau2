//lang::CwC


#pragma once


#include "../util/array.h"

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
class FieldArray : public Array {
public:
    int* starts_; // the start bytes
    int* ends_; // the end bytes

    /**
     * Default constructor.
     */
    FieldArray() : Array() {
        this->starts_ = new int[this->capacity_];
        this->ends_ = new int[this->capacity_];
    }

    /**
     * Default destructor.
     */
    ~FieldArray() override {
        delete[] this->starts_;
        delete[] this->ends_;
    }

    /**
     * Adds a new pair of start and end byte location of a field.
     * @param start the start byte
     * @param end the end byte
     */
    virtual void pushBack(int start, int end) {
        this->resizeIfFull();
        this->starts_[this->size_] = start;
        this->ends_[this->size_] = end;
        this->size_ += 1;
    }

    /**
     * Grows the array if it has reached it's capacity limit.
     */
    void resize() override {
        this->capacity_ *= 2;
        int* new_starts = new int[this->capacity_];
        int* new_ends = new int[this->capacity_];
        for(size_t i = 0; i < this->size_; ++i) {
            new_starts[i] = this->starts_[i];
            new_ends[i] = this->ends_[i];
        }
        delete[] this->starts_;
        delete[] this->ends_;
        this->starts_ = new_starts;
        this->ends_ = new_ends;
    }

    /**
     * Returns the starting byte of the field at index i.
     * @param i the index of the field in this field array.
     * @return the starting byte, or -1 if the given index is out of bound.
     */
    virtual int getStart(size_t i) {
        if (i >= this->size_) {
            return -1;
        }
        return this->starts_[i];
    }

    /**
     * Returns the ending byte of the field at index i.
     * @param i the index of the field in this field array.
     * @return the ending byte, or -1 if the given index is out of bound.
     */
    virtual int getEnd(size_t i) {
        if (i >= this->size_) {
            return -1;
        }
        return this->ends_[i];
    }
};
#pragma once


#include <cstdarg>
#include "../util/object.h"
#include "../util/string.h"
#include "../util/string_array.h"


/* Forward Declaration */
class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;


/**
 * Column ::
 *
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer equality.
 */
class Column : public Object {
public:
    char type_;
    size_t size_;
    size_t nrows_;
    const size_t rlen_ = 100;

    /**
     * Return same column as an IntColumn or nullptr if of the wrong type.
     * @return the IntColumn or nullptr if of the wrong type
     */
    virtual IntColumn *as_int() {
        return nullptr;
    }

    /**
     * Return same column as a BoolColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual BoolColumn *as_bool() {
        return nullptr;
    }

    /**
     * Return same column as a FloatColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual FloatColumn *as_float() {
        return nullptr;
    }

    /**
     * Return same column as a StringColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual StringColumn *as_string() {
        return nullptr;
    }

    /**
     * Adds the given int value at the end of this column.
     * NOTE: calling this method on a wrong typed column is
     * @param val the value to push
     */
    virtual void push_back(int val) {}

    /**
     * Adds the given bool value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void push_back(bool val) {}

    /**
     * Adds the given float value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void push_back(float val) {}

    /**
     * Adds the given String value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void push_back(String *val) {}

    /**
     * Returns the number of elements in the column.
     * @return the number of elements
     */
    virtual size_t size() {
        return this->size_;
    }

    /**
     * Return the type of this column as a char: 'S', 'B', 'I' and 'F'.
     * @return the type of this column
     */
    char get_type() {
        return this->type_;
    }

    size_t getRowIdx_(size_t idx) {
        return (size_t) (idx / this->rlen_);
    }

    size_t getColIdx_(size_t idx) {
        return idx % this->rlen_;
    }

};

/**
 * IntColumn::
 *
 * Holds primitive int values, unwrapped.
 */
class IntColumn : public Column {

public:
    int **vals_;

    /**
     * Default constructor
     */
    IntColumn() {
        this->type_ = 'I';
        this->size_ = 0;
        this->nrows_ = 1;
        this->vals_ = new int *[this->nrows_];
        this->vals_[0] = new int[this->rlen_];
    }

    /**
     * Constructs a new IntColumn from the given variable number n of integers.
     * @param n the number of integers
     * @param ... the integers
     */
    IntColumn(int n, ...) {
        this->type_ = 'I';
        this->size_ = n;
        // number of inner arrays that will be initialized
        this->nrows_ = (size_t) n / this->rlen_ + 1;
        this->vals_ = new int*[this->nrows_];

        for (size_t i = 0; i < this->nrows_; ++i) {
            this->vals_[i] = new int[this->rlen_];
        }

        va_list args;
        va_start(args, n);

        // creates all inner arrays - 1
        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
            for (size_t j = 0; j < this->rlen_; ++j) {
                this->vals_[i][j] = va_arg(args, int);
            }
        }
        // the length of the last row
        size_t last_row_len = this->getColIdx_(n);
        for (size_t i = 0; i < last_row_len; ++i) {
            this->vals_[this->nrows_ - 1][i] = va_arg(args, int);
        }

        va_end(args);
    }

    /**
     * Default destructor
     */
    ~IntColumn() override {
        // delete the inner arrays
        for (size_t i = 0; i < this->nrows_; ++i) {
            delete[] this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    int get(size_t idx) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        return this->vals_[rowIdx][colIdx];
    }

    /**
     * Returns this column as an IntColumn.
     * @return this IntColumn
     */
    IntColumn *as_int() override {
        return this;
    }

    /**
     * Sets the value at the given index with the given value
     * @param idx the index
     * @param val the value or exits the program if index is out of bound
     */
    void set(size_t idx, int val) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        this->vals_[rowIdx][colIdx] = val;
    }

    /**
     * Grows the inner number of arrays by 1.
     */
    void grow_() {
        int** new_rows = new int*[this->nrows_ + 1];
        // copy over the pointers to the old rows
        for (size_t i = 0; i < this->nrows_; ++i) {
            new_rows[i] = this->vals_[i];
        }
        // add in new row
        new_rows[this->nrows_] = new int[this->rlen_];
        delete[] this->vals_;
        this->vals_ = new_rows;
        this->nrows_++;
    }

    /**
     * Adds the given int value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void push_back(int val) override {
        size_t colIdx = this->getColIdx_(this->size_);
        // check if we need to add a new inner array to hold this new value
        if (this->size_ != 0 && colIdx == 0) {
            // pass in the current number of rows
            this->grow_();
        }

        this->vals_[this->nrows_ - 1][colIdx] = val;
        this->size_++;
    }

    /**
     * Returns a clone of this IntColumn
     * @return the clone
     */
    IntColumn* clone() override {
        IntColumn* clone = new IntColumn();
        for (size_t i = 0; i < this->size_; ++i) {
            clone->push_back(this->get(i));
        }
        return clone;
    }
};

/**
 * BoolColumn::
 *
 * Holds primitive boolean values, unwrapped.
 */
class BoolColumn : public Column {

public:
    bool **vals_;

    /**
     * Default constructor
     */
    BoolColumn() {
        this->type_ = 'B';
        this->size_ = 0;
        this->nrows_ = 1;
        this->vals_ = new bool *[this->nrows_];
        this->vals_[0] = new bool[this->rlen_];
    }

    /**
     * Constructs a new BoolColumn from the given variable number n of booleans.
     * @param n the number of booleans
     * @param ... the booleans
     */
    BoolColumn(int n, ...) {
        this->type_ = 'B';
        this->size_ = n;
        // number of inner arrays that will be initialized
        this->nrows_ = (size_t)(n / this->rlen_) + 1;
        this->vals_ = new bool*[this->nrows_];
        for (size_t i = 0; i < this->nrows_; ++i) {
            this->vals_[i] = new bool[this->rlen_];
        }
        va_list args;
        va_start(args, n);
        // creates all inner arrays - 1
        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
            for (size_t j = 0; j < this->rlen_; ++j) {
                this->vals_[i][j] = va_arg(args, int);
            }
        }
        // the length of the last row
        size_t last_row_len = this->getColIdx_(n);
        for (size_t i = 0; i < last_row_len; ++i) {
            this->vals_[this->nrows_ - 1][i] = va_arg(args, int);
        }
        va_end(args);
    }

    /**
     * Default destructor
     */
    ~BoolColumn() override {
        // delete the inner arrays
        for (size_t i = 0; i < this->nrows_; ++i) {
            delete[] this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    bool get(size_t idx) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        return this->vals_[rowIdx][colIdx];
    }

    /**
     * Returns this column as an BoolColumn.
     * @return this BoolColumn
     */
    BoolColumn *as_bool() override {
        return this;
    }

    /**
     * Sets the value at the given index with the given value
     * @param idx the index
     * @param val the value or exits the program if index is out of bound
     */
    void set(size_t idx, bool val) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        this->vals_[rowIdx][colIdx] = val;
    }

    /**
     * Grows the inner number of arrays by 1.
     */
    void grow_() {
        bool** new_rows = new bool*[this->nrows_ + 1];
        // copy over the pointers to the old rows
        for (size_t i = 0; i < this->nrows_; ++i) {
            new_rows[i] = this->vals_[i];
        }
        // add in new row
        new_rows[this->nrows_] = new bool[this->rlen_];
        delete[] this->vals_;
        this->vals_ = new_rows;
        this->nrows_++;
    }

    /**
     * Adds the given bool value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void push_back(bool val) override {
        size_t colIdx = this->getColIdx_(this->size_);
        // check if we need to add a new inner array to hold this new value
        if (this->size_ != 0 && colIdx == 0) {
            // pass in the current number of rows
            this->grow_();
        }

        this->vals_[this->nrows_ - 1][colIdx] = val;
        this->size_++;
    }

    /**
     * Returns a clone of this BoolColumn
     * @return the clone
     */
    BoolColumn* clone() override {
        BoolColumn* clone = new BoolColumn();
        for (size_t i = 0; i < this->size_; ++i) {
            clone->push_back(this->get(i));
        }
        return clone;
    }
};

/**
 * FloatColumn::
 *
 * Holds primitive float values, unwrapped.
 */
class FloatColumn : public Column {

public:
    float** vals_;

    /**
     * Default constructor
     */
    FloatColumn() {
        this->type_ = 'F';
        this->size_ = 0;
        this->nrows_ = 1;
        this->vals_ = new float *[this->nrows_];
        this->vals_[0] = new float[this->rlen_];
    }

    /**
     * Constructs a new FloatColumn from the given variable number n of floats.
     * @param n the number of floats
     * @param ... the floats
     */
    FloatColumn(int n, ...) {
        this->type_ = 'F';
        this->size_ = n;
        // number of inner arrays that will be initialized
        this->nrows_ = (size_t)(n / this->rlen_) + 1;
        this->vals_ = new float*[this->nrows_];
        for (size_t i = 0; i < this->nrows_; ++i) {
            this->vals_[i] = new float[this->rlen_];
        }
        va_list args;
        va_start(args, n);
        // creates all inner arrays - 1
        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
            for (size_t j = 0; j < this->rlen_; ++j) {
                this->vals_[i][j] = va_arg(args, double);
            }
        }
        // the length of the last row
        size_t last_row_len = this->getColIdx_(n);
        for (size_t i = 0; i < last_row_len; ++i) {
            this->vals_[this->nrows_ - 1][i] = va_arg(args, double);
        }
        va_end(args);
    }

    /**
     * Default destructor
     */
    ~FloatColumn() override {
        // delete the inner arrays
        for (size_t i = 0; i < this->nrows_; ++i) {
            delete[] this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    float get(size_t idx) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        return this->vals_[rowIdx][colIdx];
    }

    /**
     * Returns this column as an FloatColumn.
     * @return this FloatColumn
     */
    FloatColumn *as_float() override {
        return this;
    }

    /**
     * Sets the value at the given index with the given value
     * @param idx the index
     * @param val the value or exits the program if index is out of bound
     */
    void set(size_t idx, float val) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        this->vals_[rowIdx][colIdx] = val;
    }

    /**
     * Grows the inner number of arrays by 1.
     */
    void grow_() {
        float** new_rows = new float*[this->nrows_ + 1];
        // copy over the pointers to the old rows
        for (size_t i = 0; i < this->nrows_; ++i) {
            new_rows[i] = this->vals_[i];
        }
        // add in new row
        new_rows[this->nrows_] = new float[this->rlen_];
        delete[] this->vals_;
        this->vals_ = new_rows;
        this->nrows_++;
    }

    /**
     * Adds the given float value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void push_back(float val) override {
        size_t colIdx = this->getColIdx_(this->size_);
        // check if we need to add a new inner array to hold this new value
        if (this->size_ != 0 && colIdx == 0) {
            // pass in the current number of rows
            this->grow_();
        }

        this->vals_[this->nrows_ - 1][colIdx] = val;
        this->size_++;
    }

    /**
     * Returns a clone of this FloatColumn
     * @return the clone
     */
    FloatColumn* clone() override {
        FloatColumn* clone = new FloatColumn();
        for (size_t i = 0; i < this->size_; ++i) {
            clone->push_back(this->get(i));
        }
        return clone;
    }
};

/**
 * StringColumn::
 *
 * Holds string pointers. The strings are external.  Nullptr is a valid value.
 */
class StringColumn : public Column {

public:
    StringArray** vals_; // the values

    /**
     * Default constructor.
     */
    StringColumn() {
        this->type_ = 'S';
        this->size_ = 0;
        this->nrows_ = 1;
        this->vals_ = new StringArray *[this->nrows_];
        this->vals_[0] = new StringArray(this->rlen_);
    }

    /**
     * Constructs a new StringColumn from the given variable number n of string pointers.
     * @param n the number of string
     * @param ... the integers
     */
    StringColumn(int n, ...) {
        this->type_ = 'S';
        this->size_ = n;
        // number of inner arrays that will be initialized
        this->nrows_ = (size_t)(n / this->rlen_) + 1;
        this->vals_ = new StringArray*[this->nrows_];
        for (size_t i = 0; i < this->nrows_; ++i) {
            this->vals_[i] = new StringArray(this->rlen_);
        }
        va_list args;
        va_start(args, n);
        // creates all inner arrays - 1
        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
            for (size_t j = 0; j < this->rlen_; ++j) {
                this->vals_[i]->push_back(va_arg(args, String*)->clone());
            }
        }
        // the length of the last row
        size_t last_row_len = this->getColIdx_(n);
        for (size_t i = 0; i < last_row_len; ++i) {
            this->vals_[this->nrows_ - 1]->push_back(va_arg(args, String*)->clone());
        }
        va_end(args);
    }

    /**
     * Default destructor
     */
    ~StringColumn() {
        // free the inner arrays
        for (size_t i = 0; i < this->nrows_; ++i) {
            delete this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Returns this column as a StringColumn.
     * @return this StringColumn
     */
    StringColumn *as_string() override {
        return this;
    }

    /**
     * Returns the string at idx.
     * @param idx the index
     * @return the string at index or exits the program if index out of bound
     */
    String *get(size_t idx) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        return this->vals_[rowIdx]->get(colIdx);
    }


    /**
     * Acquire ownership of the string.
     * @param idx the index to set
     * @param val the value to set or exits the program if index out of bound
     */
    void set(size_t idx, String *val) {
        if (idx >= this->size_) {
            exit(1);
        }
        int rowIdx = this->getRowIdx_(idx);
        int colIdx = this->getColIdx_(idx);
        delete this->vals_[rowIdx]->get(colIdx);
        this->vals_[rowIdx]->set(colIdx, val->clone());
    }

    /**
     * Grows the inner number of arrays by 1.
     */
    void grow_() {
        StringArray** new_rows = new StringArray*[this->nrows_ + 1];
        // copy over the pointers to the old rows
        for (size_t i = 0; i < this->nrows_; ++i) {
            new_rows[i] = this->vals_[i];
        }
        // add in new row
        new_rows[this->nrows_] = new StringArray(this->rlen_);
        delete[] this->vals_;
        this->vals_ = new_rows;
        this->nrows_++;
    }

    /**
     * Adds the given String value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void push_back(String* val) override {
        // check if we need to add a new inner array to hold this new value
        if (this->size_ != 0 && (this->size_ % this->rlen_) == 0) {
            // pass in the current number of rows
            this->grow_();
        }
        if (val == nullptr) {
            this->vals_[this->nrows_ - 1]->push_back(val);
        } else {
            this->vals_[this->nrows_ - 1]->push_back(val->clone());
        }
        this->size_++;
    }

    /**
     * Finds the index of the given value in this StringColumn.
     * @param val the value
     * @return the index of the given value or -1 if not found.
     */
    int indexOf(const char *val) {
        size_t last_row_len = this->getColIdx_(this->size_);
        // check if the value is in the first n-1 rows
        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
            for (size_t j = 0; j < this->rlen_; ++j) {
                String* s = this->vals_[i]->get(j);
                // compare raw char*'s
                if (val != nullptr && strcmp(s->c_str(), val) == 0) {
                    return (int)(i * this->rlen_ + j);
                }
            }
        }
        // initialize the value to -1 in case is not found
        int result = -1;
        // check if it is in the last row
        for (size_t i = 0; i < last_row_len; ++i) {
            String* s = this->vals_[this->nrows_ - 1]->get(i);
            if (s != nullptr && strcmp(s->c_str(), val) == 0) {
                result = (int)((this->nrows_ - 1) * this->rlen_ + i);
            }
        }
        return result;
    }

    /**
     * Returns a clone of this StringColumn
     * @return the clone
     */
    StringColumn* clone() override {
        StringColumn* clone = new StringColumn();
        for (size_t i = 0; i < this->size_; ++i) {
            clone->push_back(this->get(i)->clone());
        }
        return clone;
    }
};

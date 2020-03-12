#pragma once


#include "../util/object.h"
#include "column.h"
#include "fielder.h"

/**
 * Row ::
 *
 * This class represents a single row of data constructed according to a
 * DataFrame's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a DataFrame hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
public:
    Column** vals_; // owned, and individual col are also owned
    char* schema_; // owned
    size_t index_;
    size_t nCols_;

    /** Build a row following a schema. */
    explicit Row(char* scm) {
		this->nCols_ = strlen(scm);
        this->vals_ = new Column*[this->nCols_];
		// Each case push back once to create a slot in the column
		for (size_t i = 0; i < this->nCols_; ++i) {
			switch (scm[i]) {
				case 'B':
					this->vals_[i] = new BoolColumn();
					this->vals_[i]->as_bool()->push_back(false);
					break;
				case 'I':
					this->vals_[i] = new IntColumn();
					this->vals_[i]->as_int()->push_back(0);
					break;
				case 'F':
					this->vals_[i] = new FloatColumn();
					this->vals_[i]->as_float()->push_back(0.0);
					break;
				case 'S':
					this->vals_[i] = new StringColumn();
					this->vals_[i]->as_string()->push_back(nullptr);
					break;
				default: exit(2);
			}
		}
        this->schema_ = new char[this->nCols_ + 1];
		memcpy(this->schema_, scm, this->nCols_);
        this->index_ = SIZE_MAX;
    }

    /** Destroys a row. */
    ~Row() override {
		for (size_t i = 0; i < this->nCols_; ++i) {
			delete this->vals_[i];
		}
		delete[] this->vals_;
		delete this->schema_;
    }

    /**
     * Setters: set the given column with the given value. Setting a column
     * with a value of the wrong type is undefined. All values are external.
     * Strings are not acquired.
     */
    void set(size_t col, bool val) {
        assert(this->schema_[col] == 'B');
		this->vals_[col]->as_bool()->set(0, val);
    }

    void set(size_t col, int val) {
        assert(this->schema_[col] == 'I');
		this->vals_[col]->as_int()->set(0, val);
    }

    void set(size_t col, float val) {
        assert(this->schema_[col] == 'F');
		this->vals_[col]->as_float()->set(0, val);
    }

    void set(size_t col, String* val) {
        assert(this->schema_[col] == 'S');
		this->vals_[col]->as_string()->set(0, val);
    }

    /**
     * Set/get the index of this row (ie. its position in the dataframe.
     * This is only used for informational purposes, unused otherwise.
     */
    void set_idx(size_t idx) {
        this->index_ = idx;
    }

    size_t get_idx() {
        return this->index_;
    }

    /**
     * Getters: get the value at the given column. If the column is not
     * of the requested type, the result is undefined.
     */
    bool get_bool(size_t col) {
        if (this->schema_[col] != 'B') {
			exit(1);
		}
		BoolColumn* b = dynamic_cast<BoolColumn*>(this->vals_[col]);
        return b->get(0);
    }

    int get_int(size_t col) {
        if (this->schema_[col] != 'I') {
			exit(1);
		}
		IntColumn* i = dynamic_cast<IntColumn*>(this->vals_[col]);
        return i->get(0);
    }

    float get_float(size_t col) {
        if (this->schema_[col] != 'F') {
			exit(1);
		}
		FloatColumn* f = dynamic_cast<FloatColumn*>(this->vals_[col]);
        return f->get(0);
    }

    String* get_string(size_t col) {
        if (this->schema_[col] != 'S') {
			exit(1);
		}
		StringColumn* s = dynamic_cast<StringColumn*>(this->vals_[col]);
        return s->get(0);
    }

    /**
     * Number of fields in the row.
     */
    size_t width() {
        return this->nCols_;
    }

    /**
     * Type of the field at the given position. An idx >= width is undefined.
     */
    char col_type(size_t idx) {
        return this->schema_[idx];
    }

    /**
     * Given a Fielder, visit every field of this row.
     *
     * Calling this method before the row's fields have been set is undefined.
     */
    void visit(size_t idx, Fielder& f) {
        if (idx != this->index_) {
            exit(1);
        }
        f.start(idx);
        for (size_t i = 0; i < this->nCols_; ++i) {
            switch(this->schema_[i]) {
                case 'B':
                    f.accept(this->vals_[i]->as_bool()->get(0));
                    break;
                case 'I':
                    f.accept(this->vals_[i]->as_int()->get(0));
                    break;
                case 'F':
                    f.accept(this->vals_[i]->as_float()->get(0));
                    break;
                case 'S':
                    f.accept(this->vals_[i]->as_string()->get(0));
                    break;
                default:
                    exit(1);
            }
        }
        f.done();
    }
};

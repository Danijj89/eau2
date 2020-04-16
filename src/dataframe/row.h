#pragma once


#include "fielder.h"
#include "schema.h"
#include "val.h"
#include "DFData.h"

/**
 * Row ::
 *
 * This class represents a single row of data constructed according to a
 * DataFrame's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Rows have pointer equality.
 */
class Row : public Object {
public:
    DFData* vals_; //owned
    Schema* schema_; // owned
    size_t nCols_;

    /** Build a row following a schema. */
    Row(Schema* scm) {
    	this->nCols_ = scm->width();
        this->vals_ = new DFData[this->nCols_];
		// Each case push back once to create a slot in the column
		for (size_t i = 0; i < this->nCols_; ++i) {
			this->vals_[i] = DFData();
		}
        this->schema_ = new Schema(scm);
    }

    /** Destroys a row. */
    ~Row() override {
		delete[] this->vals_;
		delete this->schema_;
    }

    /**
     * Sets the boolean at the given column index
     * @param col the index of the column to set
     * @param val the value to set
     */
    void set(size_t col, bool val) {
        assert(this->schema_->colType(col) == 'B');
        this->vals_[col].payload_.b = val;
    }

	/**
	 * Sets the integer at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, int val) {
		assert(this->schema_->colType(col) == 'I');
		this->vals_[col].payload_.i = val;
    }

	/**
	 * Sets the double at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, double val) {
		assert(this->schema_->colType(col) == 'D');
		this->vals_[col].payload_.d = val;
    }

	/**
	 * Sets the string at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, String* val) {
		assert(this->schema_->colType(col) == 'S');
		this->vals_[col].payload_.s = val;
    }

    /**
     * Get the boolean at the given column index
     * @param col the index of the column
     * @return the value
     */
    bool getBool(size_t col) {
		assert(this->schema_->colType(col) == 'B');
		return this->vals_[col].payload_.b;
    }

	/**
	 * Get the integer at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    int getInt(size_t col) {
		assert(this->schema_->colType(col) == 'I');
		return this->vals_[col].payload_.i;
    }

	/**
	 * Get the double at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    double getDouble(size_t col) {
		assert(this->schema_->colType(col) == 'D');
		return this->vals_[col].payload_.d;
    }

	/**
	 * Get the string at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    String* getString(size_t col) {
		assert(this->schema_->colType(col) == 'S');
		return this->vals_[col].payload_.s;
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
    char colType(size_t idx) {
        return this->schema_->colType(idx);
    }

    /**
     * Given a Fielder, visit every field of this row.
     *
     * Calling this method before the row's fields have been set is undefined.
     */
    void visit(Fielder* f) {
        f->start();
        for (size_t i = 0; i < this->nCols_; ++i) {
            switch(this->schema_->colType(i)) {
                case 'B':
                    f->accept(this->vals_[i].payload_.b);
                    break;
                case 'I':
					f->accept(this->vals_[i].payload_.i);
                    break;
                case 'D':
					f->accept(this->vals_[i].payload_.d);
                    break;
                case 'S':
					f->accept(this->vals_[i].payload_.s);
                    break;
                default:
                    assert(false);
            }
        }
        f->done();
    }
};

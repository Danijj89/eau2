#pragma once


#include "fielder.h"
#include "schema.h"
#include "val.h"

/**
 * Row ::
 *
 * This class represents a single row of data constructed according to a
 * DataFrame's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Rows have pointer equality.
 */
class Row : public Object {
public:
    Val** vals_; //owned
    Schema* schema_; // owned
    size_t nCols_;

    /** Build a row following a schema. */
    Row(Schema* scm) {
    	this->nCols_ = scm->width();
        this->vals_ = new Val*[this->nCols_];
		// Each case push back once to create a slot in the column
		for (size_t i = 0; i < this->nCols_; ++i) {
			switch (scm->colType(i)) {
				case 'B':
					this->vals_[i] = new Bool();
					break;
				case 'I':
					this->vals_[i] = new Int();
					break;
				case 'D':
					this->vals_[i] = new Double();
					break;
				case 'S':
					this->vals_[i] = new Str();
					break;
				default:
					assert(false);
			}
		}
        this->schema_ = new Schema(scm);
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
     * Sets the boolean at the given column index
     * @param col the index of the column to set
     * @param val the value to set
     */
    void set(size_t col, bool val) {
        assert(this->schema_->colType(col) == 'B');
        this->vals_[col]->asBool()->set(val);
    }

	/**
	 * Sets the integer at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, int val) {
		assert(this->schema_->colType(col) == 'I');
		this->vals_[col]->asInt()->set(val);
    }

	/**
	 * Sets the double at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, double val) {
		assert(this->schema_->colType(col) == 'D');
		this->vals_[col]->asDouble()->set(val);
    }

	/**
	 * Sets the string at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
    void set(size_t col, String* val) {
		assert(this->schema_->colType(col) == 'S');
		this->vals_[col]->asStr()->set(val);
    }

    /**
     * Get the boolean at the given column index
     * @param col the index of the column
     * @return the value
     */
    bool getBool(size_t col) {
		assert(this->schema_->colType(col) == 'B');
		return this->vals_[col]->asBool()->get();
    }

	/**
	 * Get the integer at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    int getInt(size_t col) {
		assert(this->schema_->colType(col) == 'I');
		return this->vals_[col]->asInt()->get();
    }

	/**
	 * Get the double at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    double getDouble(size_t col) {
		assert(this->schema_->colType(col) == 'D');
		return this->vals_[col]->asDouble()->get();
    }

	/**
	 * Get the string at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
    String* getString(size_t col) {
		assert(this->schema_->colType(col) == 'S');
		return this->vals_[col]->asStr()->get();
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
                    f->accept(this->vals_[i]->asBool()->get());
                    break;
                case 'I':
					f->accept(this->vals_[i]->asInt()->get());
                    break;
                case 'D':
					f->accept(this->vals_[i]->asDouble()->get());
                    break;
                case 'S':
					f->accept(this->vals_[i]->asStr()->get());
                    break;
                default:
                    assert(false);
            }
        }
        f->done();
    }
};

#pragma once


#include "../util/object.h"
#include "df_data_array.h"
#include "schema.h"
#include "fielder.h"


class Row : public Object {
public:
	Schema* schema_; // owned
	DFDataArray* vals_; //owned

	Row(Schema* schema) {
		this->schema_ = new Schema(schema);
		size_t nCols = this->schema_->width();
		this->vals_ = new DFDataArray(nCols);
		// Initialize array with values so that it can be set
		for (size_t i = 0; i < nCols; i++) {
			this->vals_->pushBack(new DFData(0));
		}
	}

	~Row() override {
		delete this->schema_;
		delete this->vals_;
	}

	/**
	 * Sets the boolean at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
	void set(size_t col, bool val) {
		assert(this->schema_->colType(col) == 'B');
		DFData* v = new DFData(val);
		this->vals_->set(col, v);
	}

	/**
	 * Sets the integer at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
	void set(size_t col, int val) {
		assert(this->schema_->colType(col) == 'I');
		DFData* v = new DFData(val);
		this->vals_->set(col, v);
	}

	/**
	 * Sets the double at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
	void set(size_t col, double val) {
		assert(this->schema_->colType(col) == 'D');
		DFData* v = new DFData(val);
		this->vals_->set(col, v);
	}

	/**
	 * Sets the string at the given column index
	 * @param col the index of the column to set
	 * @param val the value to set
	 */
	void set(size_t col, String* val) {
		assert(this->schema_->colType(col) == 'S');
		DFData* v = new DFData(new String(*val));
		this->vals_->set(col, v);
	}

	/**
	 * Get the boolean at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
	bool getBool(size_t col) {
		assert(this->schema_->colType(col) == 'B');
		return this->vals_->get(col)->payload_.b;
	}

	/**
	 * Get the integer at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
	int getInt(size_t col) {
		assert(this->schema_->colType(col) == 'I');
		return this->vals_->get(col)->payload_.i;
	}

	/**
	 * Get the double at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
	double getDouble(size_t col) {
		assert(this->schema_->colType(col) == 'D');
		return this->vals_->get(col)->payload_.d;
	}

	/**
	 * Get the string at the given column index
	 * @param col the index of the column
	 * @return the value
	 */
	String* getString(size_t col) {
		assert(this->schema_->colType(col) == 'S');
		return this->vals_->get(col)->payload_.s;
	}

	/**
	 * Gets the value at the given column as a DFData
	 * @param col
	 * @return
	 */
	DFData* getDFData(size_t col) {
		return this->vals_->get(col);
	}

	/**
	 * Number of fields in the row.
	 */
	size_t width() {
		return this->vals_->size();
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
		for (size_t i = 0; i < this->width(); ++i) {
			switch(this->schema_->colType(i)) {
				case 'B':
					f->accept(this->vals_->get(i)->payload_.b);
					break;
				case 'I':
					f->accept(this->vals_->get(i)->payload_.i);
					break;
				case 'D':
					f->accept(this->vals_->get(i)->payload_.d);
					break;
				case 'S':
					f->accept(this->vals_->get(i)->payload_.s);
					break;
				default:
					assert(false);
			}
		}
		f->done();
	}
};

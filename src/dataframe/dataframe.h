
#pragma once


#include <thread>
#include "column.h"
#include "row.h"
#include "rower.h"
#include "schema.h"
#include "../application/wordcount/visitor.h"

/**
 * ModifiedDataFrame::
 *
 * A ModifiedDataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A ModifiedDataFrame has a schema that
 * describes it.
 */
class DataFrame : public Object {
public:
    Schema* schema_;
    Column** vals_;
    size_t ncols_;
    size_t nrows_;

    DataFrame() {
        this->schema_ = new Schema();
        this->ncols_ = 0;
        this->nrows_ = 0;
        this->vals_ = nullptr;
    }

    ~DataFrame() {
        delete this->schema_;
        for (size_t i = 0; i < this->ncols_; ++i) {
            delete this->vals_[i];
        }
        delete[] this->vals_;
    }

    /**
     * Returns the ModifiedDataFrame's schema. Modifying the schema after a ModifiedDataFrame
     * has been created in undefined.
     * @return the schema of this ModifiedDataFrame
     */
    Schema* getSchema() {
        return this->schema_;
    }

    /**
     * Adds a column to this ModifiedDataFrame, updates the schema, the new column
     * is external, and appears as the last column of the ModifiedDataFrame.
     * If the given column is a nullptr or if the size of the column
     * does not match the number of rows in this ModifiedDataFrame,
     * we throw an error and exit.
     *
     * @param col the column to be added
     */
    void addColumn(Column* col) {
        assert(col != nullptr && (col->size() == this->nrows_ || this->nrows_ == 0));
        this->grow_();
        char t = col->get_type();
        this->vals_[this->ncols_ - 1] = col;
		this->schema_->addColumn(t);
        this->nrows_ = col->size();
    }

    /**
     * Grows the array of columns by 1.
     * This is called every time a column is added to the ModifiedDataFrame.
     */
    void grow_() {
        Column** new_vals = new Column*[this->ncols_ + 1];
        for (size_t i = 0; i < this->ncols_; ++i) {
            new_vals[i] = this->vals_[i];
        }
        delete[] this->vals_;
        this->vals_ = new_vals;
        this->ncols_ += 1;
    }

    /**
     * Return the int value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type will exit the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    int getInt(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
        || this->schema_->colType(col) != 'I') {
            exit(1);
        }
        return this->vals_[col]->asInt()->get(row);
    }

    /**
     * Return the bool value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exit the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    bool getBool(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->colType(col) != 'B') {
            exit(1);
        }
        return this->vals_[col]->asBool()->get(row);
    }

    /**
     * Return the double value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exits the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    double getDouble(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->colType(col) != 'D') {
            exit(1);
        }
        return this->vals_[col]->asDouble()->get(row);
    }

    /**
     * Return the string value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exits the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    String*  getString(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->colType(col) != 'S') {
            exit(1);
        }

        return this->vals_[col]->asString()->get(row);
    }

    /**
     * Set the fields of the given row object with values from the columns at
     * the given offset.  If the row is not from the same schema as the
     * ModifiedDataFrame, results are undefined.
     * @param idx row index in the ModifiedDataFrame
     * @param row the row object to set
     */
    void fillRow(size_t idx, Row& row) {
        for (size_t i = 0; i < this->ncols_; ++i) {
            char t = this->schema_->colType(i);
            switch(t) {
                case 'B':
                    row.set(i, this->vals_[i]->asBool()->get(idx));
                    break;
                case 'I':
                    row.set(i, this->vals_[i]->asInt()->get(idx));
                    break;
                case 'D':
                    row.set(i, this->vals_[i]->asDouble()->get(idx));
                    break;
                case 'S':
                    row.set(i, this->vals_[i]->asString()->get(idx));
                    break;
                default:
                    exit(1);
            }
        }
    }

    /**
     * The number of rows in the ModifiedDataFrame.
     * @return the number of rows.
     */
    size_t nrows() {
        return this->nrows_;
    }

    /**
     * The number of columns in the ModifiedDataFrame.
     * @return the number of columns.
     */
    size_t ncols() {
        return this->ncols_;
    }

    /**
     * Visit rows in order.
     * @param r the visitor
     */
     void map(Visitor& v) {
		Row* row = new Row(this->schema_);
        for (size_t i = 0; i < this->nrows_; ++i) {
			fillRow(i, *row);
            v.visit(*row);
        }
     }

    void localMap(Visitor* v) {}
};
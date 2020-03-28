
#pragma once


#include <thread>
#include "column.h"
#include "row.h"
#include "rower.h"
#include "schema.h"

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
    void map(Rower& r) {
        Row* row = new Row(this->schema_);
        for (size_t i = 0; i < this->nrows_; ++i) {
			fillRow(i, *row);
            r.accept(*row);
        }
    }

    /**
     * Print the ModifiedDataFrame in SoR format to standard output.
     */
    void print() {
        StrBuff* sb = new StrBuff();
        char buff[329] = {0};
        size_t len = 0;
        for (size_t i = 0; i < this->nrows_; ++i) {
            for (size_t j = 0; j < this->ncols_; ++j) {
                sb->c("<");
                switch(this->schema_->colType(j)) {
                    case 'B':
                        sb->c(this->vals_[j]->asBool()->get(i));
                        break;
                    case 'I':
                        len = sprintf(buff, "%d", this->vals_[j]->asInt()->get(i));
                        buff[len] = '\0';
                        sb->c(buff);
                        break;
                    case 'D':
                        len = sprintf(buff, "%e", this->vals_[j]->asDouble()->get(i));
                        buff[len] = '\0';
                        sb->c(buff);
                        break;
                    case 'S':
                        sb->c(*this->vals_[j]->asString()->get(i));
                        break;
                    default:
                        exit(1);
                }
                sb->c(">");
            }
            sb->c("\n");
        }
        printf("%s", sb->get()->c_str());
    }

    /**
     * Map method called by the threads where the given
     * from and to values are the beginning and ending row
     * indexes between which they should perform their work.
     * (from is inclusive, to is exclusive)
     *
     * @param from the beginning row index
     * @param to the ending row index
     * @param r the rower
     */
    void rangeMap_(size_t from, size_t to, Rower* r) {
        Row* row = new Row(this->schema_);
        for (size_t i = from; i < to; ++i) {
			fillRow(i, *row);
            r->accept(*row);
        }
    }

    /**
     * This method clones the Rower and executes the map in parallel. Join is
     * used at the end to merge the results.
     * @param r
     */
    void pmap(Rower& r) {
        std::thread** pool = new std::thread*[NUM_DF_THREADS];

        size_t chuck_size = (size_t)this->nrows_ / NUM_DF_THREADS;
        // array of row indexes delimiting the work of each thread.
        // Left inclusive, right exclusive
        size_t* work_chucks = new size_t[NUM_DF_THREADS + 1];
        work_chucks[0] = 0;
        work_chucks[NUM_DF_THREADS] = this->nrows_;

        size_t sofar = 0;
        for (size_t i = 1; i < NUM_DF_THREADS; ++i) {
            size_t end = sofar + chuck_size;
            work_chucks[i] = end;
            sofar += chuck_size;
        }

        // clone rowers
        Rower** rowers = new Rower*[NUM_DF_THREADS];
        rowers[0] = &r;
        for (size_t i = 1; i < NUM_DF_THREADS; ++i) {
            rowers[i] = dynamic_cast<Rower*>(r.clone());
        }

        // start threads with the appropriate work
        for (size_t i = 0; i < NUM_DF_THREADS; ++i) {
            pool[i] = new std::thread(&DataFrame::rangeMap_, this, work_chucks[i], work_chucks[i + 1], rowers[i]);
        }

        // wait for all threads to finish
        for (size_t i = 0; i < NUM_DF_THREADS; ++i) {
            pool[i]->join();
        }

        // join work
        for (size_t i = NUM_DF_THREADS - 1; i > 0; --i) {
			rowers[i - 1]->joinDelete(rowers[i]);
        }
        delete[] pool;
    }
};
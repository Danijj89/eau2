
#pragma once


#include <thread>
#include "../util/object.h"
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
    const size_t nthreads_ = std::thread::hardware_concurrency();

    /**
     * Create a data frame with the same columns as the give df but no rows.
     *
     * @param df the ModifiedDataFrame
     */
    DataFrame(DataFrame& df) {
        this->schema_ = new Schema(*df.get_schema());
        this->ncols_ = this->schema_->width();

        this->nrows_ = 0;
        this->vals_ = new Column*[this->ncols_];
        for (size_t i = 0; i < this->ncols_; ++i) {
            switch(this->schema_->col_type(i)) {
                case 'B':
                    this->vals_[i] = new BoolColumn();
                    break;
                case 'I':
                    this->vals_[i] = new IntColumn();
                    break;
                case 'F':
                    this->vals_[i] = new FloatColumn();
                    break;
                case 'S':
                    this->vals_[i] = new StringColumn();
                    break;
                default:
                    // a schema should always contain correct types
                    // so this should never happen
                    exit(1);
            }
        }
    }

    /**
     * Create a data frame from a schema. Columns are created empty. Results are undefined if
     * the columns do not match the schema.
     * @param schema the schema
     */
    DataFrame(Schema& schema) {
        this->schema_ = new Schema(schema);
        this->ncols_ = this->schema_->width();
        this->nrows_ = 0;
        this->vals_ = new Column*[this->ncols_];
        for (size_t i = 0; i < this->ncols_; ++i) {
            switch(this->schema_->col_type(i)) {
                case 'B':
                    this->vals_[i] = new BoolColumn();
                    break;
                case 'I':
                    this->vals_[i] = new IntColumn();
                    break;
                case 'F':
                    this->vals_[i] = new FloatColumn();
                    break;
                case 'S':
                    this->vals_[i] = new StringColumn();
                    break;
                default:
                    // a schema should always contain correct types
                    // so this should never happen
                    exit(1);
            }
        }
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
    Schema* get_schema() {
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
    void add_column(Column* col) {
        assert(col != nullptr && (col->size() == this->nrows_ || this->nrows_ == 0));
        this->grow_();
        char t = col->get_type();
        switch(t) {
            case 'B':
                this->vals_[this->ncols_ - 1] = col->as_bool()->clone();
                break;
            case 'I':
                this->vals_[this->ncols_ - 1] = col->as_int()->clone();
                break;
            case 'F':
                this->vals_[this->ncols_ - 1] = col->as_float()->clone();
                break;
            case 'S':
                this->vals_[this->ncols_ - 1] = col->as_string()->clone();
                break;
            default:
                exit(1);
        }
        this->schema_->add_column(t);
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
    int get_int(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
        || this->schema_->col_type(col) != 'I') {
            exit(1);
        }
        return this->vals_[col]->as_int()->get(row);
    }

    /**
     * Return the bool value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exit the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    bool get_bool(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'B') {
            exit(1);
        }
        return this->vals_[col]->as_bool()->get(row);
    }

    /**
     * Return the float value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exits the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    float get_float(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'F') {
            exit(1);
        }
        return this->vals_[col]->as_float()->get(row);
    }

    /**
     * Return the string value at the given column and row. Accessing rows or
     * columns out of bounds, or request the wrong type exits the program.
     * @param col the column index
     * @param row the row index
     * @return the value
     */
    String*  get_string(size_t col, size_t row) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'S') {
            exit(1);
        }

        return this->vals_[col]->as_string()->get(row);
    }

    /**
     * Set the int value at the given column and row to the given value.
     * If the column is not of the right type or the indices are out of
     * bound the program exits.
     * @param col the column index
     * @param row the row index
     * @param val the value to set
     */
    void set(size_t col, size_t row, int val) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'I') {
            exit(1);
        }
        this->vals_[col]->as_int()->set(row, val);
    }

    /**
     * Set the bool value at the given column and row to the given value.
     * If the column is not of the right type or the indices are out of
     * bound, the program exits
     * @param col the column index
     * @param row the row index
     * @param val the value to set
     */
    void set(size_t col, size_t row, bool val) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'B') {
            exit(1);
        }
        this->vals_[col]->as_bool()->set(row, val);
    }

    /**
     * Set the float value at the given column and row to the given value.
     * If the column is not of the right type or the indices are out of
     * bound, the program exits.
     * @param col the column index
     * @param row the row index
     * @param val the value to set
     */
    void set(size_t col, size_t row, float val) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'F') {
            exit(1);
        }
        this->vals_[col]->as_float()->set(row, val);
    }

    /**
     * Set the string value at the given column and row to the given value.
     * If the column is not of the right type or the indices are out of
     * bound, the result is undefined.
     * @param col the column index
     * @param row the row index
     * @param val the value to set
     */
    void set(size_t col, size_t row, String* val) {
        if (col >= this->ncols_ || row >= this->nrows_
            || this->schema_->col_type(col) != 'S') {
            exit(1);
        }
        this->vals_[col]->as_string()->set(row, val);
    }

    /**
     * Set the fields of the given row object with values from the columns at
     * the given offset.  If the row is not from the same schema as the
     * ModifiedDataFrame, results are undefined.
     * @param idx row index in the ModifiedDataFrame
     * @param row the row object to set
     */
    void fill_row(size_t idx, Row& row) {
        row.set_idx(idx);
        for (size_t i = 0; i < this->ncols_; ++i) {
            char t = this->schema_->col_type(i);
            switch(t) {
                case 'B':
                    row.set(i, this->vals_[i]->as_bool()->get(idx));
                    break;
                case 'I':
                    row.set(i, this->vals_[i]->as_int()->get(idx));
                    break;
                case 'F':
                    row.set(i, this->vals_[i]->as_float()->get(idx));
                    break;
                case 'S':
                    row.set(i, this->vals_[i]->as_string()->get(idx));
                    break;
                default:
                    exit(1);
            }
        }
    }

    /**
     * Add a row at the end of this ModifiedDataFrame. The row is expected to have
     * the right schema and be filled with values, otherwise undefined.
     * @param row the row to add
     */
    void add_row(Row& row) {
        assert(row.width() == this->ncols_);
        // check row matches the schema before starting to insert
        for (size_t i = 0; i < this->ncols_; ++i) {
            if (row.col_type(i) != this->schema_->col_type(i)) {
                exit(1);
            }
        }
        for (size_t i = 0; i < this->ncols_; ++i) {
            char t = this->schema_->col_type(i);
            switch(t) {
                case 'B':
                    this->vals_[i]->as_bool()->push_back(row.get_bool(i));
                    break;
                case 'I':
                    this->vals_[i]->as_int()->push_back(row.get_int(i));
                    break;
                case 'F':
                    this->vals_[i]->as_float()->push_back(row.get_float(i));
                    break;
                case 'S':
                    this->vals_[i]->as_string()->push_back(row.get_string(i));
                    break;
                default:
                    exit(1);
            }
        }
        this->nrows_ += 1;
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
        Row* row = new Row(*this->schema_);
        for (size_t i = 0; i < this->nrows_; ++i) {
            fill_row(i, *row);
            r.accept(*row);
        }
    }

    /**
     * Create a new ModifiedDataFrame, constructed from rows for which the given Rower
     * returned true from its accept method.
     * @param r the visitor
     * @return a new ModifiedDataFrame
     */
    DataFrame* filter(Rower& r) {
        DataFrame* result = new DataFrame(*this->schema_);
        Row* row = new Row(*this->schema_);
        for (size_t i = 0; i < this->nrows_; ++i) {
            if (r.accept(*row)) {
                result->add_row(*row);
            }
        }
        return result;
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
                switch(this->schema_->col_type(j)) {
                    case 'B':
                        sb->c(this->vals_[j]->as_bool()->get(i));
                        break;
                    case 'I':
                        len = sprintf(buff, "%d", this->vals_[j]->as_int()->get(i));
                        buff[len] = '\0';
                        sb->c(buff);
                        break;
                    case 'F':
                        len = sprintf(buff, "%e", this->vals_[j]->as_float()->get(i));
                        buff[len] = '\0';
                        sb->c(buff);
                        break;
                    case 'S':
                        sb->c(*this->vals_[j]->as_string()->get(i));
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
        Row* row = new Row(*this->schema_);
        for (size_t i = from; i < to; ++i) {
            fill_row(i, *row);
            r->accept(*row);
        }
    }

    /**
     * This method clones the Rower and executes the map in parallel. Join is
     * used at the end to merge the results.
     * @param r
     */
    void pmap(Rower& r) {
        std::thread* pool[this->nthreads_];

        size_t chuck_size = (size_t)this->nrows_ / this->nthreads_;
        // array of row indexes delimiting the work of each thread.
        // Left inclusive, right exclusive
        size_t* work_chucks = new size_t[this->nthreads_ + 1];
        work_chucks[0] = 0;
        work_chucks[this->nthreads_] = this->nrows_;

        size_t sofar = 0;
        for (size_t i = 1; i < this->nthreads_; ++i) {
            size_t end = sofar + chuck_size;
            work_chucks[i] = end;
            sofar += chuck_size;
        }

        // clone rowers
        Rower** rowers = new Rower*[this->nthreads_];
        rowers[0] = &r;
        for (size_t i = 1; i < this->nthreads_; ++i) {
            rowers[i] = dynamic_cast<Rower*>(r.clone());
        }

        // start threads with the appropriate work
        for (size_t i = 0; i < this->nthreads_; ++i) {
            pool[i] = new std::thread(&DataFrame::rangeMap_, this, work_chucks[i], work_chucks[i + 1], rowers[i]);
        }

        // wait for all threads to finish
        for (size_t i = 0; i < this->nthreads_; ++i) {
            pool[i]->join();
        }

        // join work
        for (size_t i = this->nthreads_ - 1; i > 0; --i) {
            rowers[i - 1]->join_delete(rowers[i]);
        }
    }
};
#pragma once


#include "../util/object.h"
#include "column.h"

/**
 * Schema::
 *
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
public:
    size_t clen_;
    size_t rlen_;
    IntColumn* types_; // using int array to store chars

    /**
     * Copying constructor.
     * @param from the schema to copy from.
     */
    Schema(Schema& from) {
        this->clen_ = from.width();
        this->types_ = new IntColumn();
        for (size_t i = 0; i < this->clen_; ++i) {
            this->types_->push_back(from.col_type(i));
        }
    }

    /**
     * Create an empty schema.
     */
    Schema() {
        this->clen_ = 0;
        this->rlen_ = 0;
        this->types_ = new IntColumn();
    }

    /**
     * Create a schema from a string of types. A string that contains
     * characters other than those identifying the four type results in
     * undefined behavior. The argument is external, a nullptr argument is undefined.
     * @param types the schema
     */
    Schema(const char* types) {
        if (!types) exit(1);
        this->rlen_ = 0;
        this->types_ = new IntColumn();
        size_t len = strlen(types);
        this->clen_ = len;
        for (size_t i = 0; i < len; ++i) {
            if (is_valid_type_(types[i])) {
                this->types_->push_back(types[i]);
            } else {
                exit(1);
            }
        }
    }

    /**
     * Destructor.
     */
    ~Schema() {
        delete this->types_;
    }

    /**
     * Determines whether a given type represented by a char is valid.
     * A valid type is one of the following characters:
     * - I
     * - B
     * - F
     * - S
     * @param t
     * @return
     */
    bool is_valid_type_(char t) {
        return t == 'B' || t == 'I' || t == 'F' || t == 'S';
    }

    /**
     * Add a column of the given type.
     * @param typ type of the column
     */
    void add_column(char typ) {
        if (!is_valid_type_(typ)) {
            exit(1);
        }
        this->types_->push_back(typ);
        this->clen_ += 1;
    }

    /**
     * Return type of column at idx. An idx >= width is undefined.
     * @param idx the index of the column
     * @return the type of the column
     */
    char col_type(size_t idx) {
        if (idx >= this->clen_) exit(1);
        return (char)this->types_->get(idx);
    }

    /**
     * The number of columns.
     * @return the number of columns
     */
    size_t width() {
        return this->clen_;
    }
};

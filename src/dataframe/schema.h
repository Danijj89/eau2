#pragma once

#include "../util/int_array.h"


/**
 * Schema::
 *
 * A schema is a description of the contents of a dataframe, the schema
 * knows the number of columns and the types of each column.
 * Valid types are:
 * - B: boolean
 * - I: integer
 * - D: double
 * - S: string
 */
class Schema : public Object {
public:
    IntArray* types_; // using int array to store chars

    /**
     * Copying constructor.
     * @param from the schema to copy from.
     */
    Schema(Schema* from) {
        this->types_ = new IntArray();
        size_t ncols = from->width();
        for (size_t i = 0; i < ncols; ++i) {
			this->types_->pushBack(from->colType(i));
        }
    }

    /**
     * Create an empty schema.
     */
    Schema() {
        this->types_ = new IntArray();
    }

    /**
     * Destructor.
     */
    ~Schema() {
        delete this->types_;
    }

    /**
     * Determines whether a given type represented by a char is valid.
     * @param t the type
     * @return valid or not
     */
    bool isValidType_(char t) {
        return t == 'B' || t == 'I' || t == 'D' || t == 'S';
    }

    /**
     * Add a column of the given type.
     * @param typ type of the column
     */
    void addColumn(char typ) {
        assert(this->isValidType_(typ));
		this->types_->pushBack(typ);
    }

    /**
     * Return type of column at idx. An idx >= width is undefined.
     * @param idx the index of the column
     * @return the type of the column
     */
    char colType(size_t idx) {
        assert(idx < this->width());
        return (char)this->types_->get(idx);
    }

    /**
     * The number of columns.
     * @return the number of columns
     */
    size_t width() {
        return this->types_->len();
    }
};

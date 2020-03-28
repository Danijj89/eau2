#pragma once

#include "../util/int_array.h"


/**
 * Schema::
 *
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'D'.
 */
class Schema : public Object {
public:
    size_t clen_;
    IntArray* types_; // using int array to store chars

    /**
     * Copying constructor.
     * @param from the schema to copy from.
     */
    Schema(Schema* from) {
        this->clen_ = from->width();
        this->types_ = new IntArray();
        for (size_t i = 0; i < this->clen_; ++i) {
			this->types_->pushBack(from->colType(i));
        }
    }

    /**
     * Create an empty schema.
     */
    Schema() {
        this->clen_ = 0;
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
     * A valid type is one of the following characters:
     * - I
     * - B
     * - D
     * - S
     * @param t
     * @return
     */
    bool isValiType_(char t) {
        return t == 'B' || t == 'I' || t == 'D' || t == 'S';
    }

    /**
     * Add a column of the given type.
     * @param typ type of the column
     */
    void addColumn(char typ) {
        if (!isValiType_(typ)) {
            exit(1);
        }
		this->types_->pushBack(typ);
        this->clen_ += 1;
    }

    /**
     * Return type of column at idx. An idx >= width is undefined.
     * @param idx the index of the column
     * @return the type of the column
     */
    char colType(size_t idx) {
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

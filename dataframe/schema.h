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
    StringColumn* colNames_;
    StringColumn* rowNames_;

    /**
     * Copying constructor.
     * @param from the schema to copy from.
     */
    Schema(Schema& from) {
        this->clen_ = from.width();
        this->rlen_ = from.length();
        this->types_ = new IntColumn();
        this->colNames_ = new StringColumn();
        this->rowNames_ = new StringColumn();
        for (size_t i = 0; i < this->clen_; ++i) {
            this->types_->push_back(from.col_type(i));
            this->colNames_->push_back(from.col_name(i));
            this->rowNames_->push_back(from.row_name(i));
        }
    }

    /**
     * Create an empty schema.
     */
    Schema() {
        this->clen_ = 0;
        this->rlen_ = 0;
        this->types_ = new IntColumn();
        this->colNames_ = new StringColumn();
        this->rowNames_ = new StringColumn();
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
        this->colNames_ = new StringColumn();
        this->rowNames_ = new StringColumn();
        size_t len = strlen(types);
        this->clen_ = len;
        for (size_t i = 0; i < len; ++i) {
            if (is_valid_type_(types[i])) {
                this->types_->push_back(types[i]);
                this->colNames_->push_back(nullptr);
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
        delete this->colNames_;
        delete this->rowNames_;
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
     * Add a column of the given type and name (can be nullptr), name is external.
     * @param typ type of the column
     * @param name name of the column
     */
    void add_column(char typ, String* name) {
        if (!is_valid_type_(typ)) {
            exit(1);
        }
        this->types_->push_back(typ);
        this->colNames_->push_back(name);
        this->clen_ += 1;
    }

    /**
     * Add a row with a name (possibly nullptr), name is external.
     * @param name
     */
    void add_row(String* name) {
        this->rowNames_->push_back(name);
        this->rlen_ += 1;
    }

    /**
     * Return name of row at idx; nullptr indicates no name.
     * An idx >= width is undefined.
     * @param idx the index of the row to retrieve the name for
     * @return the name of the row or nullptr if none is found
     */
    String* row_name(size_t idx) {
        if (idx >= this->rlen_) {
            return nullptr;
        }
        return this->rowNames_->get(idx);
    }

    /**
     * Return name of column at idx; nullptr indicates no name given. An idx >= width is undefined.
     * @param idx the index of the column to retrieve the name for
     * @return the name of the column or nullptr if none is found
     */
    String* col_name(size_t idx) {
        if (idx >= this->clen_) {
            return nullptr;
        }
        return this->colNames_->get(idx);
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
     * Given a column name return its index, or -1.
     * @param name the column name
     * @return the index of the column or -1 if not found
     */
    int col_idx(const char* name) {
        assert(name != nullptr);
        return this->colNames_->indexOf(name);
    }

    /**
     * Given a row name return its index, or -1.
     * @param name the row name
     * @return the index of the row or -1 if not found
     */
    int row_idx(const char* name) {
        assert(name != nullptr);
        return this->rowNames_->indexOf(name);
    }

    /**
     * The number of columns.
     * @return the number of columns
     */
    size_t width() {
        return this->clen_;
    }

    /**
     * The number of rows.
     * @return the number of rows
     */
    size_t length() {
        return this->rlen_;
    }
};

#pragma once


#include "../util/object.h"
#include "../util/string.h"
#include "../serialization/serializer.h"
#include "../serialization/deserializer.h"

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
	String* types_;

	/**
	 * Copying constructor.
	 * @param from the schema to copy from.
	 */
	Schema(Schema* from) {
		size_t nCols = from->width();
		char* types = new char[nCols + 1];
		for (size_t i = 0; i < nCols; ++i) {
			types[i] = from->colType(i);
		}
		types[nCols] = '\0';
		this->types_ = new String(types);
	}

	Schema(const char* types) {
		if (types == nullptr) {
			assert(false);
		}
		int len = strlen(types);
		for (int i = 0; i < len; ++i) {
			assert(this->isValidType_(types[i]));
		}
		this->types_ = new String(types);
	}

	/**
	 * Create an empty schema.
	 */
	Schema() {
		this->types_ = new String("");
	}

	/**
	 * Default destructor.
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
		size_t len = this->types_->size();
		char* prev = this->types_->c_str();
		char* next = new char[len + 2];
		memcpy(next, prev, len);
		next[len] = typ;
		next[len + 1] = '\0';
		delete this->types_;
		this->types_ = new String(next);
	}

	/**
	 * Return type of column at idx. An idx >= width is undefined.
	 * @param idx the index of the column
	 * @return the type of the column
	 */
	char colType(size_t idx) {
		assert(idx < this->width());
		return (char)this->types_->c_str()[idx];
	}

	/**
	 * The number of columns.
	 * @return the number of columns
	 */
	size_t width() {
		return this->types_->size();
	}

	String* serialize() {
		Serializer s = Serializer();
		return s.serializeString(this->types_)->get();
	}

	static Schema* deserialize(char* buff, size_t* counter) {
		Deserializer d = Deserializer();
		String* types = d.deserializeString(buff, counter);
		Schema* result = new Schema(types->c_str());
		delete types;
		return result;
	}
};

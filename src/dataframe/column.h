#pragma once


#include <cstdarg>
#include "../util/object.h"
#include "../util/string.h"
#include "../util/string_array.h"
#include "../kvstore/key_array.h"
#include "../kvstore/value.h"
#include "../kvstore/KVStore.h"


/* Forward Declaration */
class IntColumn;
class BoolColumn;
class FloatColumn;
class StringColumn;


/**
 * Column ::
 *
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer equality.
 */
class Column : public Object {
public:
	String* id_;
    char type_;
    size_t size_;
    KeyArray* keys_;
    Key* cache_key_;
    KVStore* store_;
    Serializer ser_;
    Deserializer des_;
    size_t max_elements_;

	/**
	 * Default constructor
	 */
	Column(String* id, KVStore* store) {
		this->id_ = id; // steals
		this->size_ = 0;
		this->keys_ = new KeyArray();
		this->cache_key_ = nullptr;
		this->store_ = store; // external
		this->ser_ = Serializer();
		this->des_ = Deserializer();
		this->max_elements_ = 0;
	}

	/**
     * Default destructor
     */
	~Column() override {
		delete this->id_;
		delete this->keys_;
	}

    /**
     * Return same column as an IntColumn or nullptr if of the wrong type.
     * @return the IntColumn or nullptr if of the wrong type
     */
    virtual IntColumn *asInt() {
        return nullptr;
    }

    /**
     * Return same column as a BoolColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual BoolColumn *asBool() {
        return nullptr;
    }

    /**
     * Return same column as a FloatColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual FloatColumn *asFloat() {
        return nullptr;
    }

    /**
     * Return same column as a StringColumn, or nullptr if of the wrong type.
     * @return the BoolColumn or nullptr if of the wrong type
     */
    virtual StringColumn *asString() {
        return nullptr;
    }

    /**
     * Adds the given int value at the end of this column.
     * NOTE: calling this method on a wrong typed column is
     * @param val the value to push
     */
    virtual void pushBack(int* val, size_t n) {}

    /**
     * Adds the given bool value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void pushBack(bool* val, size_t n) {}

    /**
     * Adds the given float value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void pushBack(float* val, size_t n) {}

    /**
     * Adds the given String value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void pushBack(StringArray* val) {}

    /**
     * Returns the number of elements in the column.
     * @return the number of elements
     */
    virtual size_t size() {
        return this->size_;
    }

    /**
     * Return the type of this column as a char: 'S', 'B', 'I' and 'F'.
     * @return the type of this column
     */
    char get_type() {
        return this->type_;
    }

	size_t getKeyIdx(size_t idx) {
		return idx / this->max_elements_;
	}

	size_t getElementIdx(size_t idx) {
		return idx % this->max_elements_;
	}

    virtual Key* getNextKey() {
    	StrBuff s = StrBuff();
    	s.c(*this->id_);
    	s.c("_");
    	s.c(this->keys_->size_);
    	return new Key(s.get());
    }

};

/**
 * IntColumn::
 *
 * Holds primitive int values, unwrapped.
 */
class IntColumn : public Column {

public:
	int* cache_value_;

    /**
     * Default constructor
     */
    IntColumn(String* id, KVStore* store) : Column(id, store) {
        this->type_ = 'I';
        this->cache_value_ = nullptr;
        this->max_elements_ = MAX_INT_ELEMENTS;
    }

//    /**
//     * Constructs a new IntColumn from the given variable number n of integers.
//     * @param n the number of integers
//     * @param ... the integers
//     */
//    IntColumn(char* id, int n, ...) : IntColumn(id) {
//        this->size_ = n;
//
//        va_list args;
//        va_start(args, n);
//
//        // creates all inner arrays - 1
//        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
//            for (size_t j = 0; j < this->rlen_; ++j) {
//                this->vals_[i][j] = va_arg(args, int);
//            }
//        }
//        // the length of the last row
//        size_t last_row_len = this->getColIdx_(n);
//        for (size_t i = 0; i < last_row_len; ++i) {
//            this->vals_[this->nrows_ - 1][i] = va_arg(args, int);
//        }
//
//        va_end(args);
//    }

	~IntColumn() {
    	delete[] this->cache_value_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    int get(size_t idx) {
    	assert(idx < this->size_);
        int keyIdx = this->getKeyIdx(idx);
        int eleIdx = this->getElementIdx(idx);
        Key* k = this->keys_->get(keyIdx);
        if (k->equals(this->cache_key_)) {
        	return this->cache_value_[eleIdx];
        }
        Value* val = this->store_->get(k);
        assert(val != nullptr);
		delete[] this->cache_value_;
		this->cache_key_ = k;
        this->cache_value_ = this->des_.deserialize_int_array(val->getBlob(), val->getSize());
        return this->cache_value_[eleIdx];
    }

    /**
     * Returns this column as an IntColumn.
     * @return this IntColumn
     */
    IntColumn *asInt() override {
        return this;
    }

    /**
     * Adds the given int value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void pushBack(int* val, size_t n) override {
    	Key* k = this->getNextKey();
    	this->ser_.serialize_int_array(val, n);
    	Value* v = new Value(this->ser_.get_buff(), n);
    	this->keys_->pushBack(k);
    	this->size_ += n;
    	this->store_->put(k, v);
    	delete k;
    	delete v;
    }
};

/**
 * BoolColumn::
 *
 * Holds primitive boolean values, unwrapped.
 */
class BoolColumn : public Column {

public:
    bool* cache_value_;

    /**
     * Default constructor
     */
    BoolColumn(String* id, KVStore* store) : Column(id, store) {
        this->type_ = 'B';
        this->cache_value_ = nullptr;
        this->max_elements_ = MAX_BOOL_ELEMENTS;
    }

//    /**
//     * Constructs a new BoolColumn from the given variable number n of booleans.
//     * @param n the number of booleans
//     * @param ... the booleans
//     */
//    BoolColumn(int n, ...) {
//        this->type_ = 'B';
//        this->size_ = n;
//        // number of inner arrays that will be initialized
//        this->nrows_ = (size_t)(n / this->rlen_) + 1;
//        this->vals_ = new bool*[this->nrows_];
//        for (size_t i = 0; i < this->nrows_; ++i) {
//            this->vals_[i] = new bool[this->rlen_];
//        }
//        va_list args;
//        va_start(args, n);
//        // creates all inner arrays - 1
//        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
//            for (size_t j = 0; j < this->rlen_; ++j) {
//                this->vals_[i][j] = va_arg(args, int);
//            }
//        }
//        // the length of the last row
//        size_t last_row_len = this->getColIdx_(n);
//        for (size_t i = 0; i < last_row_len; ++i) {
//            this->vals_[this->nrows_ - 1][i] = va_arg(args, int);
//        }
//        va_end(args);
//    }

    /**
     * Default destructor
     */
    ~BoolColumn() override {
        delete[] this->cache_value_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    bool get(size_t idx) {
		assert(idx < this->size_);
		int keyIdx = this->getKeyIdx(idx);
		int eleIdx = this->getElementIdx(idx);
		Key* k = this->keys_->get(keyIdx);
		if (k->equals(this->cache_key_)) {
			return this->cache_value_[eleIdx];
		}
		Value* val = this->store_->get(k);
		assert(val != nullptr);
		delete[] this->cache_value_;
		this->cache_key_ = k;
		this->cache_value_ = this->des_.deserialize_bool_array(val->getBlob(), val->getSize());
		return this->cache_value_[eleIdx];
    }

    /**
     * Returns this column as an BoolColumn.
     * @return this BoolColumn
     */
    BoolColumn *asBool() override {
        return this;
    }

    /**
     * Adds the given bool value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void pushBack(bool* val, size_t n) override {
		Key* k = this->getNextKey();
		this->ser_.serialize_bool_array(val, n);
		Value* v = new Value(this->ser_.get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		this->store_->put(k, v);
		delete k;
		delete v;
    }
};

/**
 * FloatColumn::
 *
 * Holds primitive float values, unwrapped.
 */
class FloatColumn : public Column {

public:
	float* cache_value_;

    /**
     * Default constructor
     */
    FloatColumn(String* id, KVStore* store) : Column(id, store) {
        this->type_ = 'F';
        this->cache_value_ = nullptr;
        this->max_elements_ = MAX_FLOAT_ELEMENTS;
    }

//    /**
//     * Constructs a new FloatColumn from the given variable number n of floats.
//     * @param n the number of floats
//     * @param ... the floats
//     */
//    FloatColumn(int n, ...) {
//        this->type_ = 'F';
//        this->size_ = n;
//        // number of inner arrays that will be initialized
//        this->nrows_ = (size_t)(n / this->rlen_) + 1;
//        this->vals_ = new float*[this->nrows_];
//        for (size_t i = 0; i < this->nrows_; ++i) {
//            this->vals_[i] = new float[this->rlen_];
//        }
//        va_list args;
//        va_start(args, n);
//        // creates all inner arrays - 1
//        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
//            for (size_t j = 0; j < this->rlen_; ++j) {
//                this->vals_[i][j] = va_arg(args, double);
//            }
//        }
//        // the length of the last row
//        size_t last_row_len = this->getColIdx_(n);
//        for (size_t i = 0; i < last_row_len; ++i) {
//            this->vals_[this->nrows_ - 1][i] = va_arg(args, double);
//        }
//        va_end(args);
//    }

    /**
     * Default destructor
     */
    ~FloatColumn() override {
        delete[] this->cache_value_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    float get(size_t idx) {
		assert(idx < this->size_);
		int keyIdx = this->getKeyIdx(idx);
		int eleIdx = this->getElementIdx(idx);
		Key* k = this->keys_->get(keyIdx);
		if (k->equals(this->cache_key_)) {
			return this->cache_value_[eleIdx];
		}
		Value* val = this->store_->get(k);
		assert(val != nullptr);
		delete[] this->cache_value_;
		this->cache_key_ = k;
		this->cache_value_ = this->des_.deserialize_float_array(val->getBlob(), val->getSize());
		return this->cache_value_[eleIdx];
    }

    /**
     * Returns this column as an FloatColumn.
     * @return this FloatColumn
     */
    FloatColumn *asFloat() override {
        return this;
    }

    /**
     * Adds the given float value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void pushBack(float* val, size_t n) override {
		Key* k = this->getNextKey();
		this->ser_.serialize_float_array(val, n);
		Value* v = new Value(this->ser_.get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		this->store_->put(k, v);
		delete k;
		delete v;
    }
};

/**
 * StringColumn::
 *
 * Holds string pointers. The strings are external.  Nullptr is a valid value.
 */
class StringColumn : public Column {

public:
    StringArray* cache_value_;

    /**
     * Default constructor.
     */
    StringColumn(String* id, KVStore* store) : Column(id, store) {
        this->type_ = 'S';
        this->cache_value_ = nullptr;
        this->max_elements_ = MAX_STRING_ELEMENTS;
    }

//    /**
//     * Constructs a new StringColumn from the given variable number n of string pointers.
//     * @param n the number of string
//     * @param ... the integers
//     */
//    StringColumn(int n, ...) {
//        this->type_ = 'S';
//        this->size_ = n;
//        // number of inner arrays that will be initialized
//        this->nrows_ = (size_t)(n / this->rlen_) + 1;
//        this->vals_ = new StringArray*[this->nrows_];
//        for (size_t i = 0; i < this->nrows_; ++i) {
//            this->vals_[i] = new StringArray(this->rlen_);
//        }
//        va_list args;
//        va_start(args, n);
//        // creates all inner arrays - 1
//        for (size_t i = 0; i < this->nrows_ - 1; ++i) {
//            for (size_t j = 0; j < this->rlen_; ++j) {
//                this->vals_[i]->push_back(va_arg(args, String*)->clone());
//            }
//        }
//        // the length of the last row
//        size_t last_row_len = this->getColIdx_(n);
//        for (size_t i = 0; i < last_row_len; ++i) {
//            this->vals_[this->nrows_ - 1]->push_back(va_arg(args, String*)->clone());
//        }
//        va_end(args);
//    }

    /**
     * Default destructor
     */
    ~StringColumn() {
        delete this->cache_value_;
    }

    /**
     * Returns this column as a StringColumn.
     * @return this StringColumn
     */
    StringColumn *asString() override {
        return this;
    }

    /**
     * Returns the string at idx.
     * @param idx the index
     * @return the string at index or exits the program if index out of bound
     */
    String *get(size_t idx) {
		assert(idx < this->size_);
		int keyIdx = this->getKeyIdx(idx);
		int eleIdx = this->getElementIdx(idx);
		Key* k = this->keys_->get(keyIdx);
		if (k->equals(this->cache_key_)) {
			return this->cache_value_->get(eleIdx);
		}
		Value* val = this->store_->get(k);
		assert(val != nullptr);
		delete this->cache_value_;
		this->cache_key_ = k;
		this->cache_value_ = this->des_.deserialize_string_array(val->getBlob(), val->getSize());
		return this->cache_value_->get(eleIdx);
    }

    /**
     * Adds the given String value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void pushBack(StringArray* val) override {
		Key* k = this->getNextKey();
		size_t n = val->len();
		this->ser_.serialize_string_array(val);
		Value* v = new Value(this->ser_.get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		this->store_->put(k, v);
		delete k;
		delete v;
    }
};

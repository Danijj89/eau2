#pragma once


#include <cstdarg>
#include "../util/object.h"
#include "../util/string.h"
#include "../util/string_array.h"
#include "../kvstore/key_array.h"
#include "../kvstore/value.h"
#include "../kvstore/KVStore.h"
#include "../serdes/serializer.h"
#include "../serdes/deserializer.h"


/* Forward Declaration */
class Serializer;
class Deserializer;
class IntColumn;
class BoolColumn;
class DoubleColumn;
class StringColumn;
class KVStore;

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
    Serializer* ser_;
    Deserializer* des_;
    size_t max_elements_;

	/**
	 * Default constructor
	 */
	Column(String* id, KVStore* store) {
		this->id_ = id; // steals
		this->type_ = '\0';
		this->size_ = 0;
		this->keys_ = new KeyArray();
		this->cache_key_ = nullptr;
		this->store_ = store; // external
		this->ser_ = new Serializer();
		this->des_ = new Deserializer();
		this->max_elements_ = 0;
	}

	Column(String* id, KVStore* store, char type, size_t size, KeyArray* keys) {
		this->id_ = id;
		this->store_ = store;
		this->type_ = type;
		this->size_ = size;
		this->keys_ = keys;
		this->cache_key_ = nullptr;
		this->ser_ = new Serializer();
		this->des_ = new Deserializer();
		this->max_elements_ = 0;
	}

	/**
     * Default destructor
     */
	~Column() override {
		delete this->id_;
		delete this->keys_;
		delete this->ser_;
		delete this->des_;
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
    virtual DoubleColumn *asDouble() {
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
     * Adds the given double value at the end of this column.
     * Calling this method on a wrong typed column is undefined behavior.
     * @param val the value to push
     */
    virtual void pushBack(double* val, size_t n) {}

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
     * Return the type of this column as a char: 'S', 'B', 'I' and 'D'.
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

    KeyArray* getKeys() {
    	return this->keys_;
    }

    String* getId() {
    	return this->id_;
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

    IntColumn(String* id, KVStore* store, size_t size, KeyArray* keys) : Column(id, store, 'I', size, keys) {
    	this->cache_value_ = nullptr;
    	this->max_elements_ = MAX_INT_ELEMENTS;
    }

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
        this->cache_value_ = this->des_->deserialize_int_array(val->getBlob(), val->getSize());
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
    	this->ser_->clear();
    	this->ser_->serialize_int_array(val, n);
    	Value* v = new Value(this->ser_->get_buff(), n);
    	this->keys_->pushBack(k);
    	this->size_ += n;
    	this->store_->put(k, v);
		int nodeId = this->store_->put(k, v);
		k->setNodeId(nodeId);
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

	BoolColumn(String* id, KVStore* store, size_t size, KeyArray* keys) : Column(id, store, 'B', size, keys) {
		this->cache_value_ = nullptr;
		this->max_elements_ = MAX_BOOL_ELEMENTS;
	}

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
		this->cache_value_ = this->des_->deserialize_bool_array(val->getBlob(), val->getSize());
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
		this->ser_->clear();
		this->ser_->serialize_bool_array(val, n);
		Value* v = new Value(this->ser_->get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		this->store_->put(k, v);
		int nodeId = this->store_->put(k, v);
		k->setNodeId(nodeId);
    }
};

/**
 * FloatColumn::
 *
 * Holds primitive double values, unwrapped.
 */
class DoubleColumn : public Column {

public:
	double* cache_value_;

    /**
     * Default constructor
     */
    DoubleColumn(String* id, KVStore* store) : Column(id, store) {
        this->type_ = 'D';
        this->cache_value_ = nullptr;
        this->max_elements_ = MAX_DOUBLE_ELEMENTS;
    }

	DoubleColumn(String* id, KVStore* store, size_t size, KeyArray* keys) : Column(id, store, 'D', size, keys) {
		this->cache_value_ = nullptr;
		this->max_elements_ = MAX_DOUBLE_ELEMENTS;
	}

    /**
     * Default destructor
     */
    ~DoubleColumn() override {
        delete[] this->cache_value_;
    }

    /**
     * Returns the value at the given index.
     * @param idx the index
     * @return the value or exits the program if index is out of bound
     */
    double get(size_t idx) {
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
		this->cache_value_ = this->des_->deserialize_double_array(val->getBlob(), val->getSize());
		return this->cache_value_[eleIdx];
    }

    /**
     * Returns this column as an FloatColumn.
     * @return this FloatColumn
     */
    DoubleColumn *asDouble() override {
        return this;
    }

    /**
     * Adds the given double value at the end of this column.
     * @param val the value to add
     *
     * INVARIANT: we always have space to hold new values unless the current size
     *            is != 0 and it is 0 when mod our rlen_, in which case we grow
     *            the number of inner arrays
     */
    void pushBack(double* val, size_t n) override {
		Key* k = this->getNextKey();
		this->ser_->clear();
		this->ser_->serialize_double_array(val, n);
		Value* v = new Value(this->ser_->get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		int nodeId = this->store_->put(k, v);
		k->setNodeId(nodeId);
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

	StringColumn(String* id, KVStore* store, size_t size, KeyArray* keys) : Column(id, store, 'S', size, keys) {
		this->cache_value_ = nullptr;
		this->max_elements_ = MAX_STRING_ELEMENTS;
	}

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
		this->cache_value_ = this->des_->deserialize_string_array(val->getBlob(), val->getSize());
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
		this->ser_->clear();
		this->ser_->serialize_string_array(val);
		Value* v = new Value(this->ser_->get_buff(), n);
		this->keys_->pushBack(k);
		this->size_ += n;
		this->store_->put(k, v);
		// Update the node id where the key has been stored
		int nodeId = this->store_->put(k, v);
		k->setNodeId(nodeId);
    }
};

void Serializer::serialize_column(Column* col) {
	this->serialize_string(col->getId());
	this->serialize_char(col->get_type());
	this->serialize_size_t(col->size());
	this->serialize_key_array(col->getKeys());
}

Column* Deserializer::deserialize_column(char* buff, size_t n, KVStore* store) {
	size_t count = 0;
	String* id = this->deserialize_string(&buff[count]);
	count += id->size() + 1;
	char type = this->deserialize_char(&buff[count]);
	count += 1;
	size_t size = this->deserialize_size_t(&buff[count]);
	count += 8;
	KeyArray* keys = this->deserialize_key_array(&buff[count], n);
	switch (type) {
		case 'B':
			return new BoolColumn(id, store, size, keys);
		case 'I':
			return new IntColumn(id, store, size, keys);
		case 'D':
			return new DoubleColumn(id, store, size, keys);
		case 'S':
			return new StringColumn(id, store, size, keys);
		default:
			assert(false);
	}
}

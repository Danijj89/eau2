//lang::CwC


/**
 * Serializer: This class will contain all the methods for serializing
 * data that will be needed to serialize a dataframe. It is used to
 * pack the body of the messages sent between nodes. It will eventually
 * support all deserialization needs of the network layer. A Serializer
 * object is meant to be reused.
 *
 * Author: zhan.d@husky.neu.edu, cao.yuan1@husky.neu.edu
 */


#pragma once


#include <cstdint>
#include <cstdlib>
#include "../network/node_info.h"
#include "../network/NodeInfoArray.h"
#include "../util/string_array.h"
#include "../kvstore/key.h"
#include "../util/constants.h"
#include "../dataframe/column.h"


class Serializer {
public:
	size_t size_;
	char* buff_; //Owned

	/**
	 * This is the constructor for the serializer.
	 * @method Serializer
	 */
	Serializer() {
		this->size_ = 0;
		this->buff_ = new char[MAX_BLOB_SIZE];
	}

	/**
	 * This is the destructor for the serializer.
	 * @method Serializer
	 */
	~Serializer() {
		delete[] this->buff_;
	}

	/**
	 * This method serializes a single boolean value.
	 * @method serialize_bool
	 * @param  val            a boolean value
	 */
	void serialize_bool(bool val) {
		this->buff_[this->size_] = val ? 1 : 0;
		this->size_++;
	}

	void serialize_bool_array(bool* vals, size_t n) {
		for (size_t i = 0; i < n; ++i) {
			this->serialize_bool(vals[i]);
		}
	}

	/**
	 * This method serializes a single char value.
	 * @method serialize_char
	 * @param  val            a char value
	 */
	void serialize_char(char val) {
		this->buff_[this->size_] = val;
		this->size_++;
	}

	void serialize_size_t(size_t val) {
		size_t s = this->size_;
		this->buff_[s + 7] = (char) (val >> 56);
		this->buff_[s + 6] = (char) (val >> 48);
		this->buff_[s + 5] = (char) (val >> 40);
		this->buff_[s + 4] = (char) (val >> 32);
		this->buff_[s + 3] = (char) (val >> 24);
		this->buff_[s + 2] = (char) (val >> 16);
		this->buff_[s + 1] = (char) (val >> 8);
		this->buff_[s] = (char) val;
		this->size_ += 8;
	}

	/**
	 * This method serializes a single int value.
	 * An int is assumed to be 4 bytes, which is reasonable on x86_64
	 * systems. It is packed into the buffer in little Endian order.
	 * Because both the client and server is developed, we do not need
	 * to worry about changing the int into network order and back.
	 * @method serialize_int
	 * @param  val           a int value.
	 */
	void serialize_int(int val) {
		size_t s = this->size_;
		this->buff_[s + 3] = (char) (val >> 24);
		this->buff_[s + 2] = (char) (val >> 16);
		this->buff_[s + 1] = (char) (val >> 8);
		this->buff_[s] = (char) val;
		this->size_ += 4;
	}

	void serialize_int_array(int* vals, size_t n) {
		for (size_t i = 0; i < n; ++i) {
			this->serialize_int(vals[i]);
		}
	}

	/**
	 * This method serializes a single float value.
	 * The order of the bytes is unchanged.
	 * @method serialize_float
	 * @param  val             a float value
	 */
	void serialize_float(float val) {
		size_t s = this->size_;
		char* v = (char*)&val;
		this->buff_[s] = v[0];
		this->buff_[s + 1] = v[1];
		this->buff_[s + 2] = v[2];
		this->buff_[s + 3] = v[3];
		this->size_ += 4;
	}

	void serialize_float_array(float* vals, size_t n) {
		for (size_t i = 0; i < n; ++i) {
			this->serialize_float(vals[i]);
		}
	}

	/**
	 * This method serializes a String.
	 * The underlying char array is exposed, and serialized.
	 * Null terminator is part of the serialization to separate the
	 * values, when this code is used to help serialize StringArrays,
	 * instead of using the size of the value.
	 * @method serialize_string
	 * @param  s                a string value
	 */
	void serialize_string(String* s) {
		char* val = s->c_str();
		for (size_t i = 0; i < s->size(); ++i) {
			this->buff_[this->size_] = val[i];
			this->size_ += 1;
		}
		this->buff_[this->size_] = '\0';
		this->size_ += 1;
	}

	/**
	 * This method serializes a StringArray.
	 * It takes advantage of the String serialization method.
	 * A Null terminator separates different Strings.
	 * @method serialize_string_array
	 * @param  val                    a StringArray value
	 */
	void serialize_string_array(StringArray* val) {
		for (size_t i = 0; i < val->len(); ++i) {
			this->serialize_string(val->get(i));
		}
	}

	/**
	 * This method serializes a NodeInfo object by serialize its String
	 * representation.
	 * @method serialize_node_info
	 * @param  ni                  a NodeInfo value
	 */
	void serialize_node_info(NodeInfo* ni) {
		this->serialize_string(ni->to_string());
	}

	/**
	 * This method serializes a NodeInfoArray object by repeatedly call
	 * serialize_node_info method.
	 * @method serialize_node_info_array
	 * @param  nia                       a NodeInfoArray value
	 */
	void serialize_node_info_array(NodeInfoArray* nia) {
		for (size_t i = 0; i < nia->len(); ++i) {
			this->serialize_node_info(nia->get(i));
		}
	}

	void serialize_key(Key* k) {
		this->serialize_string(k->getKey());
		this->serialize_int(k->getNodeId());
	}

	void serialize_key_array(KeyArray* a) {
		for (size_t i = 0; i < a->len(); ++i) {
			this->serialize_key(a->get(i));
		}
	}

	void serialize_column(Column* col) {
		this->serialize_string(col->getId());
		this->serialize_char(col->get_type());
		this->serialize_size_t(col->size());
		this->serialize_key_array(col->getKeys());
	}

	/**
	 * This methods 'clears' the buffer.
	 * Essentially forgets what was packed in there before and is ready
	 * for reuse.
	 * @method clear
	 */
	void clear() {
		this->size_ = 0;
	}

	/**
	 * This methods gets the pointer to the buffer.
	 * @method get_buff
	 * @return a pointer to the packed buffer
	 */
	char* get_buff() {
		return this->buff_;
	}

	/**
	 * This methods gets the amount that was packed into the buffer.
	 * @method get_size
	 * @return the amount of stuff packed into the buffer.
	 */
	size_t get_size() {
		return this->size_;
	}
};



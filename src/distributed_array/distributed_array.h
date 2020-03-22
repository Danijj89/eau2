#pragma once


#include "../util/object.h"
#include "../kvstore/key_array.h"
#include "../kvstore/value_array.h"

class DistributedArray : public Object {
public:
	char type_;
	KeyArray keys_; // Owned
	ValueArray myValues_; // Owned

//	Value buffer_; // Owned

//	// For storing values not yet enough for 1 Value object
//	size_t localSize_;
//	bool boolValues_[MAX_BOOL_ELEMENTS];
//	int intValues_[MAX_INT_ELEMENTS];
//	float floatValues_[MAX_FLOAT_ELEMENTS];
//	String* StringValues_[MAX_STRING_ELEMENTS];
//
//	// elements in DistributedArray
//	size_t size_;
//	size_t capacity_; // Do I need this?

	DistributedArray(char type, float) {
		this->type_ = type;

	}

	bool getBool(size_t index) {}
	int getInt(size_t index) {}
	float getFloat(size_t index) {}
	String* getString(size_t index) {}

	void pushBack(bool b) {}
	void pushBack(int i) {}
	void pushBack(float f) {}
	void pushBack(String* s) {}

	// Helpers
	void fetch(size_t index) {}
	void push(Key key) {}
	void push(Key key, Value value) {}
};

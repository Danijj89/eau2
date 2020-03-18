#pragma once


class DistributedArray : public Object {
public:
	KeyArray keys_; // Owned
	ValueArray myValues_; // Owned 
	Value buffer_; // Owned

	// For storing values not yet enough for 1 Value object
	size_t localSize_;
	bool boolValues_[BOOL_COUNT];
	int intValues_[INT_COUNT];
	float floatValues_[FLOAT_COUNT];
	String* StringValues_[STRING_COUNT];
	
	// elements in DistributedArray
	size_t size_;
	size_t capacity_; // Do I need this?

	DistributedArray() {}

	~DistributedArray() {}

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

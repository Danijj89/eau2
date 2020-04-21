#pragma once


#include "../util/object.h"
#include "../util/string.h"
#include "../serialization/serializer.h"

class Value : public Object {
public:
	String* data_; // owned

	Value() {}

	Value(String* s) {
		this->data_ = s;
	}

	~Value() {
		delete this->data_;
	}

	size_t getSize() {
		return this->data_->size();
	}

	char* getBlob() {
		return this->data_->c_str();
	}

	String* serialize() {
		Serializer s = Serializer();
		return s.serializeString(this->data_)->get();
	}

	static Value* deserialize(char* buff, size_t size, size_t* counter) {
		Value* result = new Value(new String(buff + *counter, size));
		(*counter) += size;
		return result;
	}
};

#pragma once


#include "../util/object.h"
#include "../serdes/serdes_types.h"
#include "../util/constants.h"

class Value : public Object {
public:
	char blob_[MAX_BLOB_SIZE] = {0};
	size_t size_;

	Value(char* blob, size_t size) {
		this->size_ = size;
		memcpy(&this->blob_, blob, MAX_BLOB_SIZE);
	}

	~Value() {}

	void setBlob(char* blob) {
		memcpy(&this->blob_, blob, MAX_BLOB_SIZE);
	}

	char* getBlob() {
		return this->blob_;
	}

	size_t getSize() {
		return this->size_;
	}
};

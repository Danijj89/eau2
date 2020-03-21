#pragma once


#include "../util/object.h"
#include "../serdes/serdes_types.h"
#include "../util/constants.h"

class Value : public Object {
public:
	SerDesTypes type_;
	char blob_[MAX_BLOB_SIZE];

	Value() {}

	~Value() {}

	void setBlob(char* blob, size_t size) {
		assert(size <= MAX_BLOB_SIZE);
		memcpy(&this->blob_, blob, size);
	}
};

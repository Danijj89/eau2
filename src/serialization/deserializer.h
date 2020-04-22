#pragma once

#include "../util/object.h"
#include "../kvstore/value.h"

class Deserializer : public Object {
public:

	bool deserializeBool(char* buff, size_t* counter) {
		return this->deserializeInt(buff + *counter, counter) == 1;
	}

	int deserializeInt(char* buff, size_t* counter) {
		int result = ((int*) (buff + *counter))[0];
		(*counter) += 4;
		return result;
	}

	size_t deserializeSizeT(char* buff, size_t* counter) {
		size_t result = ((size_t*) (buff + *counter))[0];
		(*counter) += 8;
		return result;
	}

	double deserializeDouble(char* buff, size_t* counter) {
		double result = ((double*) (buff + *counter))[0];
		(*counter) += 8;
		return result;
	}

	String* deserializeString(char* buff, size_t* counter) {
		String* result = new String(buff + *counter);
		(*counter) += result->size() + 1;
		return result;
	}
};
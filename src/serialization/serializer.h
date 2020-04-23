#pragma once


#include "../util/object.h"
#include "../util/string.h"

class Serializer : public Object {
public:
	StrBuff sb = StrBuff();

	String* get() {
		String* s = this->sb.get();
		return s;
	}

	Serializer* serializeBool(bool val) {
		val ? this->serializeInt(1) : this->serializeInt(0);
		return this;
	}

	Serializer* serializeInt(int val) {
		char* buff = new char[5];
		buff[3] = (char) (val >> 24);
		buff[2] = (char) (val >> 16);
		buff[1] = (char) (val >> 8);
		buff[0] = (char) val;
		buff[4] = '\0';
		String s(true, buff, 4);
		this->sb.c(s);
		return this;
	}

	Serializer* serializeSizeT(size_t val) {
		char* buff = new char[9];
		buff[7] = (char) (val >> 56);
		buff[6] = (char) (val >> 48);
		buff[5] = (char) (val >> 40);
		buff[4] = (char) (val >> 32);
		buff[3] = (char) (val >> 24);
		buff[2] = (char) (val >> 16);
		buff[1] = (char) (val >> 8);
		buff[0] = (char) val;
		buff[8] = '\0';
		String s(true, buff, 8);
		this->sb.c(s);
		return this;
	}

	Serializer* serializeDouble(double val) {
		String s((char*)&val, 8);
		this->sb.c(s);
		return this;
	}

	Serializer* serializeString(String* val) {
		this->sb.c(*val);
		this->sb.c("\0", 1);
		return this;
	}
};
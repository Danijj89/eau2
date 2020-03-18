#pragma once


class Value : public Object {
public:
	SerDesType type_;
	char blob[CONST_SIZE];

	Value() {}

	~Value() {}

	void setBlob(char* blob, size_t size) {}
};

#pragma once


#include "../util/object.h"
#include "msg_kind.h"
#include "../serialization/data_type.h"
#include "../kvstore/value.h"

/**
 * Message: This class represents a message sent through our network.
 */
class Message : public Object {
public:
	size_t size_;
	MsgKind kind_;
	DataType type_;

	char* body_; // owned


	Message(MsgKind kind, DataType type, String* v) {
		this->size_ = v->size();
		this->kind_ = kind;
		this->type_ = type;
		char* buff = new char[this->size_ + 1];
		memcpy(buff, v->c_str(), this->size_ + 1);
		this->body_ = buff;
	}

	Message(MsgKind kind, DataType type) {
		this->kind_ = kind;
		this->type_ = type;
		this->size_ = 0;
		this->body_ = nullptr;
	}

	~Message() {
		delete[] this->body_;
	}

	MsgKind getKind() { return this->kind_; }

	DataType getType() { return this->type_; }

	char* getBody() { return this->body_; }

	size_t getSize() { return this->size_; }

	void addBody(char* buff, size_t size) {
		delete[] this->body_;
		this->size_ = size;
		this->body_ = buff;
	}
};
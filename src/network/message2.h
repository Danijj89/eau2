#pragma once


#include "msg_kind.h"
#include "../serdes/serdes_types.h"
#include "../util/object.h"
#include "../util/constants.h"
#include "../serdes/serializer.h"


/**
 * This is the Message class used for variable length messages.
 */
class Message2 : public Object {
public:
	MsgKind kind_;
	SerDesTypes type_;
	size_t memberSize_;
	size_t memberArea_;
	char* body_; // Owned

	Message(MsgKind kind) {
		this->kind_ = kind;
		this->type_ = SerDesTypes::NONE;
		this->memberSize_ = 0;
		this->memberArea_ = 0;
		this->body_ = nullptr;
	}

	virtual ~Message() {
		delete this->body_;
	}

	MsgKind getKind() {
		return this->kind_;
	}

	SerDesTypes getType() {
		return this->type_;
	}

	size_t getMemberSize() {
		return this->memberSize_;
	}


	size_t getMemberArea() {
		return this->memberArea_;
	}

	char* getBody() {
		return this->body_;
	}

	void packBody(char* buff, size_t buffSize, size_t elemSize, SerDesTypes type) {
		// Steals the buff
		this->body_ = buff;
		this->memberSize_ = elemSize;
		this->memberArea_ = buffSize;
		this->type_ = type;
	}

	void addBody(char* buff) {
		this->body_ = buff;
	}
};
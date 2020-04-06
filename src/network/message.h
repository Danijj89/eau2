#pragma once


#include "msg_kind.h"
#include "../serdes/serdes_types.h"
#include "../util/object.h"
#include "../util/constants.h"
#include "../serdes/serializer.h"

class Message : public Object {
public:
	MsgKind kind_;
	SerDesTypes type_;
	size_t ser_member_size_;
	char body_[MAX_BUFF_SIZE] = {0}; // serialized information, part of message

	Message(MsgKind kind) {
		this->kind_ = kind;
		this->type_ = SerDesTypes::NONE;
		this->ser_member_size_ = 0;
	}

	Message() {
		this->kind_ = MsgKind::Empty;
		this->type_ = SerDesTypes::NONE;
		this->ser_member_size_ = 0;
	}

	virtual ~Message() {}

	void pack_body(char* buff, size_t len, SerDesTypes type, size_t members) {
		this->type_ = type;
		this->ser_member_size_ = members;
		// Copy in the entire buff, which is expected to be a Serializer buffer
		for (size_t i = 0; i < len || i < MAX_BUFF_SIZE; ++i) {
			this->body_[i] = buff[i];
		}
	}

	MsgKind get_kind() {
		return kind_;
	}

	SerDesTypes get_type() {
		return type_;
	}

	size_t get_ser_member_size() {
		return ser_member_size_;
	}

	char* get_body() {
		return body_;
	}

	void pack_put_message(Key* k, Value* v) {
		this->kind_ = MsgKind::Put;
		Serializer s = Serializer();
		s.serialize_key_value_pair(k, v);
		this->pack_body(s.get_buff(), s.get_size(), SerDesTypes::PAIR, 1);
	}

	void pack_addkey_message(Key* k) {
		this->kind_ = MsgKind::AddKey;
		Serializer s = Serializer();
		s.serialize_key(k);
		this->pack_body(s.get_buff(), s.get_size(), SerDesTypes::KEY, 1);
	}
};

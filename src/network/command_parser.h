//lang:Cwc

#pragma once


#include "msg_kind.h"
#include "node_info.h"
#include "../util/constants.h"
#include "../util/string_array.h"
#include "../util/helper.h"

class CommandParser {
public:
	MsgKind kind_;
	NodeInfo* target_;
	char* data_;
	size_t data_size_;

	CommandParser() {
		this->data_ = new char[MAX_BUFF_SIZE];
		this->data_size_ = 0;
	}

	~CommandParser() {
		delete this->target_;
		delete[] this->data_;
	}

	void parse_command(char* buff) {
		StringArray* tokens = tokenize(buff, ' ');
		char* msg_kind = tokens->get(0)->c_str();
		if (strcmp(msg_kind, "register") == 0) {
			this->handle_register(tokens);
		} else if (strcmp(msg_kind, "put") == 0) {
			this->handle_put(tokens);
		} else if (strcmp(msg_kind, "shutdown") == 0) {
			this->handle_shutdown();
		} else {
			printf("Invalid command.\n");
		}
		delete tokens;
	}

	void handle_register(StringArray* tokens) {
		this->kind_ = MsgKind::Register;
		String* target_ip = tokens->get(1);
		int target_port = atoi(tokens->get(2)->c_str());
		this->target_ = new NodeInfo(target_ip->clone(), target_port);
	}

	void handle_put(StringArray* tokens) {
		this->kind_ = MsgKind::Put;
		String* target_ip = tokens->get(1);
		int target_port = atoi(tokens->get(2)->c_str());
		this->target_ = new NodeInfo(target_ip->clone(), target_port);
		this->data_size_ = tokens->get(3)->size();
		char* val = tokens->get(3)->c_str();
		for (size_t i = 0; i < this->data_size_ && i < MAX_BUFF_SIZE; ++i) {
			this->data_[i] = val[i];
		}
	}

	void handle_shutdown() {
		this->kind_ = MsgKind::Shutdown;
	}

	MsgKind get_kind() {
		return kind_;
	}

	NodeInfo* get_target() {
		return target_;
	}

	char* get_data() {
		return data_;
	}

	size_t get_data_size() {
		return this->data_size_;
	}

};
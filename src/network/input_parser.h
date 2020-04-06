//lang:CwC


#pragma once


#include "../util/string.h"
#include "../util/constants.h"
#include "../util/string_array.h"
#include "../util/helper.h"

class InputParser {
public:
	String* ipv4_; // Owned
	int port_;

	InputParser() {
		this->ipv4_ = nullptr;
		this->port_ = SERVER_PORT;
	}

	~InputParser() {
		delete this->ipv4_;
	}

	void parse_input(int argc, char** argv) {
		assert(argc == 3);
		if (argc != 3 || strcmp(argv[1], "-ip") != 0) {
			printf("Usage: ./client -ip [String: ip_address]");
			exit(1);
		}
		StringArray* tokenized = tokenize(argv[2], ':');
		this->ipv4_ = tokenized->get(0)->clone();
		char* ip = this->ipv4_->c_str();

		// validate ip address format
		// TODO: Better validation of IP
		for (size_t i = 0; i < this->ipv4_->size(); ++i) {
			if (!isdigit(ip[i]) && ip[i] != '.') {
				printf("Invalid ip address format. Required IPv4, given: %s\n", ip);
				exit(1);
			}
		}
		// TODO: need to validate port number
		this->port_ = atoi(tokenized->get(1)->c_str());

		delete tokenized;
	}

	String* get_ip() {
		return this->ipv4_;
	}

	int get_port() {
		return this->port_;
	}
};
//lang:CwC

#pragma once

#include "../util/object.h"
#include "../util/helper.h"
#include "helper.h"

/**
 * Argument parser used to parse command line inputs for the sorer application.
 * @authors zhan.d@husky.neu.edu & yuan.cao@husky.neu.edu
 */
class ArgumentParser : public Object {
public:
	char* filename_; // file name
	size_t len_; // num bytes to read
	size_t from_; // byte to read from
	char* command_; // command to execute
	size_t val1_; // first val
	size_t val2_; // second val

	/**
	 * Default constructor.
	 */
	ArgumentParser() {
		this->filename_ = nullptr;
		this->len_ = 0;
		this->from_ = 0;
		this->command_ = nullptr;
		this->val1_ = 0;
		this->val2_ = 0;
	}

	/**
	 * Parses the inputs to the program given the argc and argv.
	 * @param argc the argc
	 * @param argv the argv
	 */
	void parse(int argc, char** argv) {
		int i = 1;
		while (i < argc) {
			if (strcmp(argv[i], "-f") == 0 && !this->filename_ && argc > i + 1) {
				this->filename_ = argv[i + 1];
				i += 2;
			} else if (strcmp(argv[i], "-from") == 0 && !this->from_ && argc > i + 1) {
				this->from_ = parse_uint(argv[i + 1]);
				i += 2;
			} else if (strcmp(argv[i], "-len") == 0 && !this->len_ && argc > i + 1) {
				this->len_ = parse_uint(argv[i + 1]);
				i += 2;
			} else if (strcmp(argv[i], "-print_col_type") == 0 && !this->command_ && argc > i + 1) {
				this->command_ = argv[i];
				this->val1_ = parse_uint(argv[i + 1]);
				i += 2;
			} else if (strcmp(argv[i], "-print_col_idx") == 0 && !this->command_ && argc > i + 2) {
				this->command_ = argv[i];
				this->val1_ = parse_uint(argv[i + 1]);
				this->val2_ = parse_uint(argv[i + 2]);
				i += 3;
			} else if (strcmp(argv[i], "-is_missing_idx") == 0 && !this->command_ && argc > i + 2) {
				this->command_ = argv[i];
				this->val1_ = parse_uint(argv[i + 1]);
				this->val2_ = parse_uint(argv[i + 2]);
				i += 3;
			} else {
				printf("%s\n", USAGE);
				exit(1);
			}
		}
	}
};
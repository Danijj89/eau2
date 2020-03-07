//lang:CwC

#pragma once

#include "../util/object.h"
#include "../util/helper.h"
#include "constants.h"

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

	~ArgumentParser() override {
		delete[] this->filename_;
		delete[] this->command_;
	}

	/**
	 * Parses the inputs to the program given the argc and argv.
	 * @param argc the argc
	 * @param argv the argv
	 */
	void parse(int argc, char** argv) {
		// Make sure that the number of arguments given are in the correct range
		if (argc < 5 || argc > 10) {
			this->print_usage();
			exit(1);
		}
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
				this->print_usage();
				exit(1);
			}
		}
	}

	void print_usage() {
		printf("%s\n", USAGE);
	}

	/**
	 * Gee the file name.
	 * @return the file name.
	 */
	char *getFilename() {
		return filename_;
	}

	/**
	 * Get the -len argument.
	 * @return the len argument.
	 */
	size_t getLen() {
		return len_;
	}

	/**
	 * Get the -from argument.
	 * @return the from argument.
	 */
	size_t getFrom() {
		return from_;
	}

	/**
	 * Get the command to execute.
	 * @return the command to execute.
	 */
	char *getCommand() {
		return command_;
	}

	/**
	 * Get the first input value for the command to execute.
	 * @return the first input of the command.
	 */
	size_t getVal1() {
		return val1_;
	}

	/**
	 * Get the second input value for the command to execute.
	 * @return the second input of the command.
	 */
	size_t getVal2() {
		return val2_;
	}
};
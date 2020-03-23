//lang:CwC
#pragma once


#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include "types_array.h"
#include "field_array.h"
#include "../dataframe/column.h"
#include "../dataframe/dataframe.h"
#include "../dataframe/schema.h"
#include "../util/constants.h"

/**
 * Class that encapsulate all logic to parse a schema on read file.
 * @authors zhan.d@husky.neu.edu & yuan.cao1@husky.neu.edu
 */
class Sorer : public Object {
public:
	int fd_;
	size_t ask_;
	size_t from_;
	size_t end_;
	size_t fileSize_;
	char *file_;
	TypesArray *schema_;
	FieldArray **columnar_;
	Column **data_;

	/**
	 * Default constructor.
	 */
	Sorer() {
		this->fd_ = -1;
		this->ask_ = 0;
		this->from_ = 0;
		this->end_ = 0;
		this->fileSize_ = 0;
		this->file_ = nullptr;
		this->schema_ = nullptr;
		this->columnar_ = nullptr;
		this->data_ = nullptr;
	}

	/**
	 * Default destructor.
	 */
	~Sorer() override {
		this->reset();
	}

	/**
	 * Resets this Sorer so it can be re-initialized to work on a different file.
	 */
	void reset() {
		if (this->fd_ >= 0) {
			munmap(this->file_, this->ask_);
			close(this->fd_);
		}
		if (this->columnar_ != nullptr) {
			for (size_t i = 0; i < this->schema_->len(); i++) {
				delete this->columnar_[i];
			}
		}
		delete[] this->columnar_;
		if (this->data_ != nullptr) {
			for (size_t i = 0; i < this->schema_->len(); i++) {
				delete this->data_[i];
			}
		}
		delete[] this->data_;
		delete this->schema_;
	}

	/**
	 * Initialize this Sorer to accept reading request with a given file path,
	 * the position to parse from and the number of bytes to parse.
	 *
	 * @param filename the file name
	 * @param from the starting byte to parse from
	 * @param len the number of bytes to parse
	 */
	void initialize_sorer(char *filename, size_t from, size_t len) {
		this->reset();
		this->from_ = from;
		this->setUpFile(filename);
		this->inferSchema_();
		this->reposition_delimiters_(len);
		this->makeColumnar_();
		this->parseData_();
	}

	/**
	 * Sets up the file to be parsed.
	 *
	 * @param filename the path to the file.
	 */
	void setUpFile(char *filename) {
		// Make sure the file exists/can be opened
		this->fd_ = open(filename, O_RDONLY);
		assert(this->fd_ != -1);

		// Use stat the get the file size
		struct stat st;
		stat(filename, &st);
		this->fileSize_ = st.st_size;

		// Make sure that the given from is not over the file size
		assert(this->from_ < this->fileSize_);

		// Get the number of page sizes we need
		int pg_size = getpagesize();
		size_t pages = (size_t) (this->fileSize_ / pg_size) + 1;
		this->ask_ = pages * pg_size;

		//Mmap for lazy read
		this->file_ = (char *) mmap(nullptr, this->ask_, PROT_READ, MAP_PRIVATE, this->fd_, 0);
	}

	/**
	 * Determines the position of the starting and ending byte.
	 * These two positions delimits the region of the file the Sorer will parse.
	 *
	 * @param len the number of bytes to parse
	 */
	void reposition_delimiters_(size_t len) {
		// discard the first line if given from != 0
		if (this->from_ != 0 && this->from_ < this->fileSize_) {
			while (this->file_[this->from_] != '\n') {
				assert(this->file_[this->from_] != EOF);
				this->from_++;
			}
			this->from_++;
		}
		// if the file size is smaller than the bytes we are supposed to read,
		// we just read the whole file
		this->end_ = this->from_ + len;

		if (this->end_ >= this->fileSize_ || len == 0) {
			this->end_ = this->fileSize_;
		} else {
			// else we discard the last line
			while (this->file_[this->end_] != '\n') {
				assert(this->end_ > this->from_);
				this->end_--;
			}
		}
	}

	/**
	 * Determines the schema of a .sor file by analyzing the first
	 * 'lines' number of lines from the beginning of the file.
	 * INVARIANT: the result TypesArray always stores the correct schema at the beginning of
 	 * every iteration. Thus, only in the case of finding a longer row or where
 	 * less restrictive fields are found, is this result array ever changed.
 	 * CREDIT: to SnowySong team for the schema parsing algorithm.
	 */
	void inferSchema_() {
		// stores the schema state so far
		this->schema_ = new TypesArray();
		size_t start = 0;
		// types array to use to store the row schema
		// it is used for all the necessary iterations and only deleted
		// at completion
		TypesArray *curr = new TypesArray();
		for (size_t i = 0; i < INFER_SCHEMA_LINES; ++i) {
			if (this->file_[start] == EOF || this->file_[start] == '\0') {
				break;
			}
			// pass the curr type array down so to store the row schema
			this->parseRowSchema_(&start, curr);
			size_t result_len = this->schema_->len();
			size_t curr_len = curr->len();
			for (size_t j = 0; j < curr_len; ++j) {
				Types curr_type = curr->get(j);
				// if the current line has more element than the one we accumulated so far,
				// it means this is the longest row and we add those types into our result
				if (j >= result_len) {
					this->schema_->pushBack(curr_type);
					// change the type of a column of the so far array only if the
					// curr type is less restrictive (meaning it has higher value
					// in the enum)
				} else if (this->schema_->get(j) < curr_type) {
					this->schema_->set(j, curr_type);
				}
			}
			// reset the curr array so it can be reused
			curr->clear();
			// move cursor to next line
			start += 1;
		}
		// we can finally delete curr
		delete curr;
	}


	/**
	 * Parses the schema of a row (line) starting at start in the file.
 	 * Stores the parsed schema into a given array of types.
 	 * MUTATION: the start argument is mutated so that it always holds the byte
 	 *           where this row parsing ended. The row_schema argument is mutated to store
 	 *           the types of this row.
 	 *
	 * @param start the starting byte to start parsing from.
	 * @param row_schema the array of types that we use to store the schema and we recycle afterwards.
	 */
	void parseRowSchema_(size_t *start, TypesArray *row_schema) {
		while (this->file_[*start] != '\n' && this->file_[*start] != EOF && this->file_[*start] != '\0') {
			// find the starting byte of the field
			if (this->file_[*start] == '<') {
				size_t field_end = *start + 1;
				// find the ending byte of the field
				while (this->file_[field_end] != '>') {
					assert(this->file_[field_end] != '\n');
					++field_end;
				}
				Types field_type = this->parseFieldType_(*start, field_end);
				row_schema->pushBack(field_type);
				*start = field_end + 1;
			} else {
				*start += 1;
			}
		}
	}


	/**
	 * Parses the type of the field delimited by its delimiters '<' and '>' pointed
 	 * by start and end respectively in the given file.
 	 *
	 * @param start the byte position of '<'
	 * @param end the byte position of '>'
	 * @return the type of this field.
	 */
	Types parseFieldType_(size_t start, size_t end) {
		Types result = Types::BOOL;

		// removes empty spaces in front and back
		start = this->triml_(start, end);
		end = this->trimr_(start, end);

		// If start is <= end we know that it is not an empty field
		if (start <= end) {
			if (this->file_[start] == '\"') {
				// must be a string
				result = Types::STRING;
			} else if (end == start && (this->file_[start] == '0' || this->file_[start] == '1')) {
				result = Types::BOOL;
			} else {
				char *endptr;
				strtoll(&this->file_[start], &endptr, 10);
				// Make sure we get a number back and that it consume all of the relevant field characters.
				if ((size_t) (endptr - &this->file_[start]) == end - start + 1) {
					return Types::INT;
				}
				strtold(&this->file_[start], &endptr);
				if ((size_t) (endptr - &this->file_[start]) == end - start + 1) {
					return Types::FLOAT;
				}
				for (size_t i = start; i <= end; ++i) {
					if (isspace(this->file_[i])) {
						return result;
					}
				}
				result = Types::STRING;
			}
		}
		return result;
	}

	/**
	 * Returns the index of the first non space character from a given 'start'
 	 * to an given 'end' in a 'file'.
 	 *
	 * @param start the starting byte.
	 * @param end the ending byte.
	 * @return the byte at which the first non space character is found.
	 */
	size_t triml_(size_t start, size_t end) {
		size_t i;
		for (i = start + 1; i < end; i++) {
			if (!isspace(this->file_[i])) {
				break;
			}
		}
		return i;
	}

	/**
 	 * Returns the index of the first non space character from a given
 	 * 'end' to a given 'start' in a 'file'.
 	 *
 	 * @param start the byte to end to.
 	 * @param end the byte to start from.
 	 * @return the byte at which the first non space character is found.
 	 */
	inline size_t trimr_(size_t start, size_t end) {
		// make sure we don't read before the start of the file.
		assert(end != 0);
		size_t i;
		for (i = end - 1; i > start; i--) {
			if (!isspace(this->file_[i])) {
				break;
			}
		}
		return i;
	}

	/**
 	 * Creates a columnar representation of a portion of a file delimited by the given
 	 * start and end according to a schema.
 	 *
 	 * NOTE: the function assumes that start always points to the beginning of a line
 	 *       and the end to the end of a line.
 	 */
	void makeColumnar_() {
		size_t max_fields = this->schema_->len();
		this->columnar_ = new FieldArray *[max_fields];
		for (size_t i = 0; i < max_fields; ++i) {
			this->columnar_[i] = new FieldArray();
		}
		// initialized the arrays that are going to be recycled every iteration
		TypesArray *row_types = new TypesArray();
		FieldArray *row_fields = new FieldArray();
		while (this->from_ < this->end_) {
			while (this->file_[this->from_] != '\n' && this->file_[this->from_] != EOF &&
				   this->file_[this->from_] != '\0') {
				size_t row_start = this->from_;
				// first parse the schema and determine if the row is valid
				this->parseRowSchema_(&row_start, row_types);

				if (this->isValidRow_(row_types)) {
					// if valid we parse the row fields and add it to our columnar
					this->parseRowFields_(row_fields);
					for (size_t i = 0; i < max_fields; ++i) {
						this->columnar_[i]->pushBack(row_fields->getStart(i), row_fields->getEnd(i));
					}
					// recycle the row_fields array
					row_fields->clear();
				}
				// recycle the row_types array
				row_types->clear();
				// move cursor to point to next line
				this->from_ = row_start + 1;
			}
		}
		// we are done so we delete them
		delete row_types;
		delete row_fields;
	}

	/**
 	 * Determines if the types row are valid according to a schema passed in.
 	 * A row is valid if and only if both the number of fields in the row and their types
 	 * respectively matches those of the schema.
 	 *
 	 * @param row_types the types array of the row to check.
 	 * @return whether it is valid or not.
 	 */
	bool isValidRow_(TypesArray *row_types) {
		size_t max_fields = this->schema_->len();
		if (row_types->len() == max_fields) {
			for (size_t i = 0; i < max_fields; ++i) {
				if (row_types->get(i) > this->schema_->get(i)) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	/**
 	 * Parses through a row (line) of a file and builds a field array
 	 * storing all the starting and ending bytes delimited by '<' and '>' of the field in the row.
 	 * MUTATION: the start argument is mutated so that it always holds the byte
 	 *           where this row parsing ended. The row argument is mutated to store
 	 *           the the starting and ending bytes found.
 	 *
 	 * @param row the field array to fill with the starting and ending bytes found.
 	 */
	void parseRowFields_(FieldArray *row) {
		while (this->file_[this->from_] != '\n' && this->file_[this->from_] != EOF &&
			   this->file_[this->from_] != '\0') {
			// find the starting byte of the field
			if (this->file_[this->from_] == '<') {
				size_t field_end = this->from_ + 1;
				// find the ending byte of the field
				while (this->file_[field_end] != '>') {
					assert(this->file_[field_end] != '\n');
					field_end++;
				}
				row->pushBack(this->from_, field_end);
				this->from_ = field_end + 1;
			} else {
				this->from_ += 1;
			}
		}
	}

	/**
	 * Parses the columnar representation into data stored in Columns.
	 */
	void parseData_() {
		size_t numCols = this->schema_->len();
		size_t numRows = this->columnar_[0]->len();
		this->data_ = new Column *[numCols];
		for (size_t i = 0; i < numCols; i++) {
			int start, end;
			switch (this->schema_->get(i)) {
				case Types::BOOL:
					this->data_[i] = new BoolColumn();
					for (size_t j = 0; j < numRows; ++j) {
						start = this->columnar_[i]->getStart(j);
						end = this->columnar_[i]->getEnd(j);
						this->data_[i]->asBool()->pushBack(this->parseBool(start, end));
					}
					break;
				case Types::INT:
					this->data_[i] = new IntColumn();
					for (size_t j = 0; j < numRows; ++j) {
						start = this->columnar_[i]->getStart(j);
						end = this->columnar_[i]->getEnd(j);
						this->data_[i]->asInt()->pushBack(this->parseInt(start, end));
					}
					break;
				case Types::FLOAT:
					this->data_[i] = new FloatColumn();
					for (size_t j = 0; j < numRows; ++j) {
						start = this->columnar_[i]->getStart(j);
						end = this->columnar_[i]->getEnd(j);
						this->data_[i]->asFloat()->pushBack(this->parseFloat(start, end));
					}
					break;
				case Types::STRING:
					this->data_[i] = new StringColumn();
					for (size_t j = 0; j < numRows; ++j) {
						start = this->columnar_[i]->getStart(j);
						end = this->columnar_[i]->getEnd(j);
						String *s = this->parseString(start, end);
						this->data_[i]->asString()->pushBack(s);
						delete s;
					}
					break;
				default:
					assert(false);
			}
		}
	}

	/**
 	 * Attempts to parse field between start and end into an bool.
 	 * Only call this function on fields that has been determined to be bool.
 	 *
 	 * @param start is the start of the field in file.
 	 * @param end is the end of the field in file.
 	 * @return the parsed bool.
 	 */
	int parseBool(size_t start, size_t end) {
		// removes empty spaces in front and back
		size_t new_start = this->triml_(start, end);
		size_t new_end = this->trimr_(start, end);
		// either they point to same byte or the end has gone
		// over because the field is missing value
		assert(new_end - new_start == 0 || new_start - new_end == 1);
		return this->file_[new_start] == '1';
	}

	/**
 	 * Attempts to parse field between start and end into an int.
 	 * Only call this function on fields that has been determined to be int.
 	 *
 	 * @param start is the start of the field in file.
 	 * @param end is the end of the field in file.
 	 * @return the parsed int.
 	 */
	int parseInt(size_t start, size_t end) {
		// removes empty spaces in front and back
		size_t new_start = this->triml_(start, end);
		size_t new_end = this->trimr_(start, end);

		if (new_start <= new_end) {
			char *endptr;
			long candidate = strtol(&this->file_[new_start], &endptr, 10);
			// Make sure we get a number back and that it consume all of the relevant field characters.
			assert((size_t) (endptr - &this->file_[new_start]) == new_end - new_start + 1);
			return (int) candidate;
		} else {
			return 0;
		}
	}

	/**
 	 * Attempts to parse field between start and end into a float.
 	 * Only call this function on fields that has been determined to be float.
 	 *
 	 * @param start is the start of the field in file.
 	 * @param end is the end of the field in file.
 	 * @return the parsed float.
 	 */
	float parseFloat(size_t start, size_t end) {
		// removes empty spaces in front and back
		size_t new_start = this->triml_(start, end);
		size_t new_end = this->trimr_(start, end);

		if (new_start < new_end) {
			char *endptr;
			long double candidate = strtold(&this->file_[new_start], &endptr);
			// Make sure we get a number back and that it consume all of the relevant field characters.
			assert((size_t) (endptr - &this->file_[new_start]) == new_end - new_start + 1);
			return (float) candidate;
		} else {
			return 0.0;
		}
	}

	/**
 	 * Attempts to parse field between start and end into a String.
 	 * Only call this function on fields that has been determined to be String.
 	 *
 	 * @param start is the start of the field in file.
 	 * @param end is the end of the field in file.
 	 * @return the parsed float.
 	 */
	String *parseString(size_t start, size_t end) {
		// removes empty spaces in front and back
		size_t new_start = this->triml_(start, end);
		size_t new_end = this->trimr_(start, end);

		if (new_start < new_end) {
			// Remove the quotes if found
			if (this->file_[new_start] == '"' && this->file_[new_end] == '"') {
				new_start += 1;
				new_end -= 1;
			}
			return new String(&this->file_[new_start], new_end - new_start + 1);
		} else {
			return new String("");
		}
	}

	/**
	 * Prints the value at given column and row.
	 *
	 * @param col the column.
	 * @param row the row.
	 */
	void printField(int col, int row) {
		Types type = this->schema_->get(col);
		switch (type) {
			case Types::BOOL:
				printf("%d\n", this->data_[col]->asBool()->get(row));
				break;
			case Types::INT:
				printf("%d\n", this->data_[col]->asInt()->get(row));
				break;
			case Types::FLOAT:
				printf("%f\n", this->data_[col]->asFloat()->get(row));
				break;
			case Types::STRING:
				printf("%s\n", this->data_[col]->asString()->get(row)->c_str());
				break;
			default:
				assert(false);
		}
	}

	/**
	 * Prints 1 or 0 depending on if the value at the given column and row is missing.
	 *
	 * @param col the column.
	 * @param row the row.
	 */
	void printIsMissing(int col, int row) {
		size_t start = this->columnar_[col]->getStart(row);
		size_t end = this->columnar_[col]->getEnd(row);
		if (end - start < 2) {
			printf("1\n");
		} else {
			printf("0\n");
		}
	}

	/**
	 * Prints the type of a column.
	 *
	 * @param col the column index.
	 */
	void printColType(size_t col) {
		switch (this->schema_->get(col)) {
			case Types::BOOL:
				printf("BOOL\n");
				break;
			case Types::INT:
				printf("INT\n");
				break;
			case Types::FLOAT:
				printf("FLOAT\n");
				break;
			case Types::STRING:
				printf("STRING\n");
				break;
			default:
				printf("Bad type encountered, this value should not be stored\n");
				exit(1);
		}
	}

	DataFrame* getAsDF() {
		Schema* s = new Schema();
		DataFrame* df = new DataFrame(*s);
		for (size_t i = 0; i < this->schema_->len(); i++) {
			df->add_column(this->data_[i]);
		}
		delete s;
		return df;
	}
};
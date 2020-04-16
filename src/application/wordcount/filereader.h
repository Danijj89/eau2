#pragma once

#include "../../dataframe/row.h"
#include "../../dataframe/rower.h"
#include "writer.h"

class FileReader : public Writer {
public:
	static const size_t BUFSIZE = 1024;
	char * buf_;
	size_t end_ = 0;
	size_t i_ = 0;
	FILE * file_;

	/** Creates the reader and opens the file for reading.  */
	explicit FileReader(const char* file) {
		this->file_ = fopen(file, "r");
		assert(this->file_ != nullptr);
		this->buf_ = new char[BUFSIZE + 1]; //  null terminator
		this->fillBuffer_();
		this->skipWhitespace_();
	}

	/** Reads next word and stores it in the row. Actually read the word.
		While reading the word, we may have to re-fill the buffer  */
	void visit(Row & r) override {
		assert(this->i_ < this->end_);
		assert(! isspace(this->buf_[this->i_]));
		size_t wStart = this->i_;
		while (true) {
			if (this->i_ == this->end_) {
				if (feof(this->file_)) { ++this->i_;  break; }
				this->i_ = wStart;
				wStart = 0;
				this->fillBuffer_();
			}
			if (isspace(this->buf_[this->i_]))  break;
			++this->i_;
		}
		this->buf_[this->i_] = 0;
		String word(this->buf_ + wStart, this->i_ - wStart);
		r.set(0, &word);
		++this->i_;
		this->skipWhitespace_();
	}

	/** Returns true when there are no more words to read.  There is nothing
	   more to read if we are at the end of the buffer and the file has
	   all been read.     */
	bool done() override { return (this->i_ >= this->end_) && feof(this->file_);  }

	/** Reads more data from the file. */
	void fillBuffer_() {
		size_t start = 0;
		// compact unprocessed stream
		if (this->i_ != this->end_) {
			start = this->end_ - this->i_;
			memcpy(this->buf_, this->buf_ + this->i_, start);
		}
		// read more contents
		this->end_ = start + fread(this->buf_+start, sizeof(char), BUFSIZE - start, this->file_);
		this->i_ = start;
	}

	/** Skips spaces.  Note that this may need to fill the buffer if the
		last character of the buffer is space itself.  */
	void skipWhitespace_() {
		while (true) {
			if (this->i_ == this->end_) {
				if (feof(this->file_)) return;
				this->fillBuffer_();
			}
			// if the current character is not whitespace, we are done
			if (!isspace(this->buf_[this->i_]))
				return;
			// otherwise skip it
			++this->i_;
		}
	}
};
#pragma once


// #include <unistd.h>
#include <poll.h>
#include <fcntl.h> // for nonblocking if necessary
#include "../util/array.h"


class Pollfds : public Array {
public:
	pollfd* vals_; // what this points to is owned

	Pollfds() {
		this->vals_ = new pollfd[this->capacity_];
	}

	~Pollfds() {
		delete [] this->vals_;
	}

	void resize() {
		this->capacity_ *= 2;
		pollfd* new_vals = new pollfd[this->capacity_];
		for (size_t i = 0; i < this->size_; ++i) {
			new_vals[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = new_vals;
	}

	pollfd* getPfds() { return this->vals_; }

	/**
	 * Pushes the given item to the end of this array.
	 *
	 * @param item the given item to be added to the end of this array
	 */
	void pushBack(int fd) {
		this->resizeIfFull();
		this->vals_[this->size_].fd = fd;
		this->vals_[this->size_].events = POLLIN;
		this->size_ += 1;
	}

	void remove(size_t idx) {
		assert(this->size_ > 0);
		this->vals_[idx] = this->vals_[this->size_ - 1];
		this->size_ -= 1;
	}
};
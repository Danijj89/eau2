#pragma once

#include <cstdlib>
#include <poll.h>
#include <cassert>

class Pollfds {
public:
	pollfd* vals_;
	size_t size_;
	size_t capacity_;

	Pollfds() {
		this->size_ = 0;
		this->capacity_ = 4;
		this->vals_ = new pollfd[this->capacity_];
	}

	~Pollfds() {
		delete[] this->vals_;
	}

	size_t size() { return this->size_; }

	void resize_() {
		this->capacity_ *= 2;
		pollfd* new_vals = new pollfd[this->capacity_];
		for (size_t i = 0; i < this->size_; ++i) {
			new_vals[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = new_vals;
	}

	pollfd* getPollfds() { return this->vals_; }

	void pushBack(int fd) {
		if (this->size_ == this->capacity_) this->resize_();
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
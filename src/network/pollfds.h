#pragma once


// #include <unistd.h>
#include <poll.h>
#include <fcntl.h> // for nonblocking


class Pollfds {
public:
	pollfd* pfds_; // what this points to is owned
	size_t size_;
	size_t capacity_;

	Pollfds(int listener, int server) {
		this->size_ = 3;
		this->capacity_ = 10;
		this->pfds_ = new pollfd[this->capacity_];

		// Set up stdin
		this->pfds_[0].fd = 0;
		this->pfds_[0].events = POLLIN;
		// Make stdin nonblocking
		// fcntl(0, F_SETFL, O_NONBLOCK);

		// Set up listening
		this->pfds_[1].fd = listener;
		this->pfds_[1].events = POLLIN;

		// Set up server
		this->pfds_[2].fd = server;
		this->pfds_[2].events = POLLIN;
	}

	~Pollfds() {
		/* Testing a non-pointer based design
		for (size_t i = 0; i < this->size_; ++i) {
			delete pfds_[i];
		}
		*/
		delete [] this->pfds_;
	}

	size_t size() { return this->size_; }

	size_t capacity() { return this->capacity_; }

	pollfd* pfds() { return this->pfds_; }

	void add_to_pfds(int newfd) {
		if (this->size_ == this->capacity_) {
			this->capacity_ *= 2;
			pollfd* new_pfds = new pollfd[this->capacity_];
			for (size_t i = 0; i < this->size_; ++i) {
				new_pfds[i] = this->pfds_[i];
			}
			delete[] this->pfds_;
			this->pfds_ = new_pfds;
		}

		// pollfd* new_poll = new pollfd(); // Non pointer
		// new_poll->fd = newfd;
		this->pfds_[this->size_].fd = newfd;
		this->pfds_[this->size_].events = POLLIN;
		this->size_ += 1;
	}

	void del_from_pfds(size_t pos) {
		// Make sure char** names works the same way if I use that

		// Reserve 0 for stdin, 1 for listening
		assert(pos > 1);
		// delete pfds_[pos]; // Trying a non-pointer based design
		this->pfds_[pos] = this->pfds_[this->size_ - 1];
		// pfds_[this->size_ - 1] = nullptr; // Non-pointer based design
		this->size_ -= 1;
	}
};
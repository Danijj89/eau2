
#pragma once

#include <unistd.h>
#include "../util/string.h"

class NodeInfo : public Object {
public:
	String* ip_; // owned
	int port_;
	int fd_;
	String* rep_; // owned

	/**
	 * Creates a new NodeInfo with the given ip address and port.
	 * Steals the String ip.
	 * @param ip
	 * @param port
	 */
	NodeInfo(String* ip, int port) {
		this->ip_ = ip;
		this->port_ = port;
		this->fd_ = -1;
		StrBuff* sb = new StrBuff();
		sb->c(*this->ip_);
		sb->c(":");
		sb->c(this->port_);
		this->rep_ = sb->get();
		delete sb;
	}

	/**
	 * Creates a new NodeInfo with the given ip address, port, and fd.
	 * @param ip
	 * @param port
	 * @param fd
	 */
	NodeInfo(String* ip, int port, int fd) : NodeInfo(ip, port) {
		this->fd_ = fd;
	}

	/**
	 * Destructor
	 */
	~NodeInfo() {
		delete this->ip_;
		delete this->rep_;
		// Close the fd if not -1
		if (this->fd_ >= 3) {
			//TODO: might not need to check that the operation was successful
			assert(close(this->fd_) == 0);
		}
	}

	String* get_ip() {
		return ip_;
	}


	int get_port() {
		return port_;
	}

	int get_fd() {
		return this->fd_;
	}

	void set_fd(int fd) {
		this->fd_ = fd;
	}

	String* to_string() {
		return this->rep_;
	}

	bool equals(NodeInfo* o) {
		NodeInfo* other = dynamic_cast<NodeInfo*>(o);
		if (other == nullptr) {
			return false;
		}
		return this->ip_ == other->get_ip() && this->port_ == other->get_port();
	}

};

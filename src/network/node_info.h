//lang: CwC
#pragma once

#include <unistd.h>
#include "../util/object.h"
#include "../util/string.h"
#include "../serialization/serializer.h"
#include "../serialization/deserializer.h"

class NodeInfo : public Object {
public:
	String* ip_; // owned
	int port_;
	int fd_;

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
		if (this->fd_ >= 3 && close(this->fd_) != 0) {
			perror("Error: ");
			assert(false);
		}
	}

	String* getIP() { return ip_; }

	int getPort() { return port_; }

	int getFd() { return this->fd_; }

	void setFd(int fd) { this->fd_ = fd; }

	bool equals(Object* o) override {
		NodeInfo* other = dynamic_cast<NodeInfo*>(o);
		if (other == nullptr) {
			return false;
		}
		return this->ip_->equals(other->getIP()) && this->port_ == other->getPort();
	}

	String* serialize() {
		Serializer s = Serializer();
		s.serializeString(this->ip_);
		s.serializeInt(this->port_);
		return s.get();
	}

	static NodeInfo* deserialize(char* buff, size_t* counter) {
		Deserializer d = Deserializer();
		String* ip = d.deserializeString(buff, counter);
		int port = d.deserializeInt(buff, counter);
		return new NodeInfo(ip, port);
	}
};

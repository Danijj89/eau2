#pragma once

#include "../util/object.h"
#include "../util/string.h"

class NodeConfiguration : public Object {
public:
	String* ip_;
	int port_;
	size_t numNodes_;
	int socketBacklog_;
	String* serverIP_;
	int serverPort_;
	char* filePath_;
	size_t chuckItems_;

	String* getIP() { return this->ip_; }

	void setIP(String* ip) { this->ip_ = ip; }

	int getPort() { return this->port_; }

	void setPort(int port) { this->port_ = port; }

	bool getIsServer() { return this->ip_->equals(this->serverIP_) && this->port_ == this->serverPort_; }

	size_t getNumNodes() { return this->numNodes_; }

	void setNumNodes(size_t n) { this->numNodes_ = n; }

	int getSocketBacklog() { return this->socketBacklog_; }

	void setSocketBacklog(int n) { this->socketBacklog_ = n; }

	String* getServerIP() { return this->serverIP_; }

	void setServerIP(String* ip) { this->serverIP_ = ip; }

	int getServerPort() { return this->serverPort_; }

	void setServerPort(int port) { this->serverPort_ = port; }

	char* getFilePath() { return this->filePath_; }

	void setFilePath(char* fp) { this->filePath_ = fp; }

	size_t getChunkItems() { return this->chuckItems_; }

	void setChunkItems(size_t n) { this->chuckItems_ = n; }
};

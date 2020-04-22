#pragma once


#include "../util/object.h"
#include "../util/string.h"


/**
 * NodeConfiguration: This class handles the configuration of the nodes used
 * in the eau2 application. Each field must be set by the user before its
 * corresponding getter can be called. The user has the ownership of all
 * objects passed into a NodeConfiguration object.
 */
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

	void setIP(String* ip) { this->ip_ = ip; }
	String* getIP() { return this->ip_; }

	void setPort(int port) { this->port_ = port; }
	int getPort() { return this->port_; }

	void setNumNodes(size_t n) { this->numNodes_ = n; }
	size_t getNumNodes() { return this->numNodes_; }

	void setSocketBacklog(int n) { this->socketBacklog_ = n; }
	int getSocketBacklog() { return this->socketBacklog_; }

	void setServerIP(String* ip) { this->serverIP_ = ip; }
	String* getServerIP() { return this->serverIP_; }

	void setServerPort(int port) { this->serverPort_ = port; }
	int getServerPort() { return this->serverPort_; }

	void setFilePath(char* fp) { this->filePath_ = fp; }
	char* getFilePath() { return this->filePath_; }

	void setChunkItems(size_t n) { this->chuckItems_ = n; }
	size_t getChunkItems() { return this->chuckItems_; }

	bool getIsServer() { return this->ip_->equals(this->serverIP_) && this->port_ == this->serverPort_; }
};

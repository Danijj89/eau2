// All network related C/C++ code should go here to keep the rest CwC

#pragma once


#include <sys/socket.h>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include "../util/string.h"
#include "message.h"
#include "node_info.h"

int getNewFd() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(fd >= 0);
	int opt = 1;
	assert(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0);
	assert(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == 0);
	return fd;
}

void bindFdToAddress(int fd, NodeInfo* info) {
	sockaddr_in adr;
	int address_len = sizeof(adr);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = inet_addr(info->getIP()->c_str());
	adr.sin_port = htons(info->getPort());
	assert(bind(fd, (struct sockaddr *) &adr, address_len) >= 0);
}

int connectTo(NodeInfo* from, NodeInfo* to) {
	String* ip = to->getIP();
	int port = to->getPort();

	int fd = getNewFd();
	// set node fd to the new fd
	to->setFd(fd);
	bindFdToAddress(fd, from);

	sockaddr_in node_adr;
	int addrlen = sizeof(node_adr);
	// Creating socket file descriptor

	node_adr.sin_family = AF_INET;
	node_adr.sin_port = htons(port);
	// Convert IP addresses from text to binary form
	assert(inet_pton(AF_INET, ip->c_str(), &node_adr.sin_addr) > 0);
	assert(connect(fd, (struct sockaddr *) &node_adr, addrlen) >= 0);
	printf("Connection established to node %s:%d on socket %d.\n", ip->c_str(), port, fd);
	return fd;
}

char* readNBytes(int socket, size_t bytesToRead) {
	char* buff = new char[bytesToRead];
	size_t bytesRead = 0;
	while (bytesRead < bytesToRead) {
		bytesRead += read(socket, buff + bytesRead, bytesToRead - bytesRead);
		if (bytesRead == 0) {
			delete[] buff;
			return nullptr;
		}
	}
	assert(bytesRead == bytesToRead);
	return buff;
}

Message* readMessage(int socket) {
	Message* m = (Message*)readNBytes(socket, sizeof(Message));
	if (m == nullptr) return new Message(MsgKind::SHUTDOWN, DataType::EMPTY);
	Message* result = new Message(m->getKind(), m->getType(), new String(""));
	result->addBody(readNBytes(socket, m->getSize()), m->getSize());
	delete[] (char*)m;
	return result;
}

void sendNBytes(int socket, char* buff, size_t bytesToSend) {
	size_t bytesSent = 0;
	while (bytesSent < bytesToSend) {
		size_t bytes = write(socket, buff + bytesSent, bytesToSend - bytesSent);
		bytesSent += bytes;
	}
	assert(bytesSent == bytesToSend);
}

void sendMessage(int socket, Message* m) {
	size_t headerSize = sizeof(Message);
	size_t bodySize = m->getSize();
	size_t totalSize = headerSize + bodySize;
	char* buff = new char[totalSize];
	memcpy(buff, (char*)m, headerSize);
	memcpy(buff + headerSize, m->getBody(), bodySize);
	sendNBytes(socket, buff, totalSize);
	delete[] buff;
}

NodeInfo* acceptConnection(int fd) {
	assert(listen(fd, 3) >= 0);
	sockaddr_in empty_sockaddr;
	int new_sock;
	socklen_t addrlen = sizeof(empty_sockaddr);
	new_sock = accept(fd, (sockaddr *) &empty_sockaddr, &addrlen);
	if (new_sock < 0) {
		return nullptr;
	}
	String *ip = new String(inet_ntoa(empty_sockaddr.sin_addr));
	int port = ntohs(empty_sockaddr.sin_port);
	printf("Accepted connection from %s:%d in socket %d\n", ip->c_str(), port, new_sock);
	return new NodeInfo(ip, port, new_sock);
}
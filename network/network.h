// All network related C/C++ code should go here to keep the rest CwC

#pragma once


#include <sys/socket.h>
#include <cassert>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../util/string.h"
#include "node_info.h"
#include "message.h"

int get_new_fd() {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(fd >= 0);
	int opt = 1;
	assert(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0);
	assert(setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) == 0);
	return fd;
}

void bind_fd_to_adr(String* ip, int port, int fd) {
	sockaddr_in adr;
	int address_len = sizeof(adr);
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = inet_addr(ip->c_str());
	adr.sin_port = htons(port);
	assert(bind(fd, (struct sockaddr *) &adr, address_len) >= 0);
}

int connect_to(NodeInfo* from, NodeInfo* to) {
	String* ip = to->get_ip();
	int port = to->get_port();

	int fd = get_new_fd();
	// set node fd to the new fd
	to->set_fd(fd);
	bind_fd_to_adr(from->get_ip(), from->get_port(), fd);

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

// We need to separate connection to server from connection to node since server does not need new fd
void connect_to_server(NodeInfo* from, NodeInfo* server) {
	char* server_ip = server->get_ip()->c_str();
	int server_port = server->get_port();
	int node_socket = from->get_fd();
	sockaddr_in node_adr;
	int addrlen = sizeof(node_adr);
	// Creating socket file descriptor

	node_adr.sin_family = AF_INET;
	node_adr.sin_port = htons(server_port);
	// Convert IP addresses from text to binary form
	assert(inet_pton(AF_INET, server_ip, &node_adr.sin_addr) > 0);
	assert(connect(node_socket, (struct sockaddr *) &node_adr, addrlen) >= 0);
	printf("Connection established to server %s:%d on socket %d.\n", server_ip, server_port, node_socket);
}

void read_message(int socket, Message* m) {
	int read_bytes = read(socket, m, sizeof(Message));
	assert(read_bytes == sizeof(Message));
}

void send_message(int socket, Message* m) {
	int write_bytes = write(socket, m, sizeof(Message));
	assert(write_bytes == sizeof(Message));
}

NodeInfo* accept_connection(int fd) {
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
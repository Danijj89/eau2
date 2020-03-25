#pragma once


#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>


#define BACKLOG 10 // OS might limit to ~20 anyway
#define BUFSIZE 1024 // Max line length
#define DEFAULT_SERVER_IP const_cast<char*>("127.0.0.1")
#define DEFAULT_PORT const_cast<char*>("12345")


/**
 * Gets the struct addrinfo lists from getaddrinfo call, asking for SOCK_STREAM.
 *
 * @param host is hostname or IP string.
 * @param service is one of "http" or a port number in string form.
 * @return the pointer for the first in the linked list of struct addrinfo.
 */
inline addrinfo* get_addr_info(char* host, char* service) {
	// Set up hints
	addrinfo hints;
	memset(&hints, 0, sizeof hints); // Zero out struct
	hints.ai_family = AF_UNSPEC; // Want both IPv4 and IPv6 if available
	hints.ai_socktype = SOCK_STREAM; // Want a TCP stream socket
	hints.ai_flags = AI_PASSIVE; //Ignored if host is not NULL, desired if is

	addrinfo *servinfo; // User is responsible for freeing list attached to this

	int status;
	if ((status = getaddrinfo(host, service, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	return servinfo;
}

/**
 * Get the IPv4 information from potential address informations.
 *
 * @param addrs is the head of the linked list of addrinfo structs.
 * @return pointer to the addrinfo with IPv4 information, or nullptr if none
 * available.
 */
inline addrinfo* get_IPv4_info(addrinfo* addrs) {
	for (addrinfo* p = addrs; p != nullptr; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			return p;
		}
	}
	return nullptr;
}

/**
 * Get the IPv6 information from potential address informations.
 *
 * @param addrs is the head of the linked list of addrinfo structs.
 * @return pointer to the addrinfo with IPv6 information, or nullptr if none
 * available.
 */
inline addrinfo* get_IPv6_info(addrinfo* addrs) {
	for (addrinfo* p = addrs; p != nullptr; p = p->ai_next) {
		if (p->ai_family == AF_INET6) {
			return p;
		}
	}
	return nullptr;
}

/**
 * Get the socket file descriptor from system
 *
 * @param domain is passed in using addrinfo, do not hand craft
 * @param type is passed in using addrinfo, do not hand craft
 * @param protocol is passed in using addrinfo, do not hand craft
 * @return file descriptor of the socket.
 */
inline int get_sockfd(int domain, int type, int protocol) {
	int sockfd;
	sockfd = socket(domain, type, protocol);
	if (sockfd == -1) {
		fprintf(stderr, "Could not get a socket file descriptor.\n");
		exit(1);
	}
	return sockfd;

}

/**
 * Set port for reuse for easier testing and being nice
 *
 * @param sockfd is the socket file descriptor we'll be using
 * @param info is the address information
 * @return success information
 */
inline void set_socket_reuse(int sockfd) {
	int success = 1;
	success = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &success, sizeof(int));
	if (success == -1) {
		fprintf(stderr, "Could not set socket options.\n");
		exit(1);
	}
}

/**
 * Bind socket
 *
 * @param sockfd is the socket file descriptor
 * @param info is the addrinfo
 */
inline void bind_socket(int sockfd, addrinfo* info) {
	int success;
	success = bind(sockfd, info->ai_addr, info->ai_addrlen);
	if (success == -1) {
		close(sockfd);
		fprintf(stderr, "Could not bind socket.\n");
		exit(1);
	}
}

/**
 * Set how many incoming connections is held in queue
 *
 * @param sockfd is the socket file descriptor
 * @param backlog is the max number of incoming connections we can hold in queue
 */
inline void set_backlog(int sockfd, int backlog) {
	int success;
	success = listen(sockfd, backlog);
	if (success == -1) {
		close(sockfd);
		fprintf(stderr, "Could not bind socket.\n");
		exit(1);
	}
}

/**
 * Handle child signals if using a process based handling
 *
 * @param signal
 */
inline void sigchld_handler(int signal) {
	fprintf(stderr, "Caught %d signal.\n", signal);
	int saved_errno = errno; // Saving errno in case it becomes important
	while (waitpid(-1, NULL, WNOHANG) > 0) {
		; // Looping forever waiting
	}
	errno = saved_errno;
}

/**
 * Get sockaddr for both IPv4 and IPv6
 *
 * @param sa is socket address
 * @return pointer to the memory region that holds the address
 */
inline void* get_in_addr(sockaddr* sa) {
	if (sa->sa_family == AF_INET) {
		return &(((sockaddr_in*)sa)->sin_addr);
	}
	return &(((sockaddr_in6*)sa)->sin6_addr);
}

/**
 * Connect to server via socket file descriptor
 *
 * @param sockfd is the socket file descriptor
 * @param info is the address info of the server
 */
inline void connect_to(int sockfd, addrinfo* info) {
	int success;
	success = connect(sockfd, info->ai_addr, info->ai_addrlen);
	if (success == -1) {
		close(sockfd);
		fprintf(stderr, "Connect failed.\n");
	}
}

/**
 * Tell user about its hostname
 */

inline void print_hostname() {
	char hostname[64];
	int success = gethostname(hostname, 64);
	if (success == -1) {
		fprintf(stderr, "I don't know who I am.");
	} else {
		printf("server: I am %s\n", hostname);
	}
}

/**
 * Write to target the message held in message
 *
 * @param sockfd is the target socket file descriptor.
 * @param message is the pointer to message buffer.
 */
inline void write_to(int sockfd, char* message) {
	int remainingbytes = strlen(message);
	int writebytes = 0;
	while (remainingbytes > 0) {
		writebytes = write(sockfd, message, remainingbytes);
		if (writebytes == -1) {
			fprintf(stderr, "Could not write to target fd.\n");
			exit(1);
		}
		message += writebytes;
		remainingbytes -= writebytes;
	}
}

/**
 * Read from sockfd and put message in message buffer
 *
 * @param sockfd is the target socket file descriptor.
 * @param message is the pointer to message buffer.
 */
inline void read_from(int sockfd, char* message, int message_size) {
	int max_size = message_size;
	int readbytes;
	readbytes = read(sockfd, message, max_size);
	if (readbytes == -1) {
		fprintf(stderr, "Could not read from target fd.\n");
		exit(1);
	} else if (readbytes == 0) {
		close(sockfd);
	}
	message[readbytes] = 0;
	message += readbytes;
	max_size -= readbytes;
}

/**
 * Set up listener
 *
 * @param ip is the ip address that listener listen from
 * @return sockfd fully setup
 */
inline int setup_listener(char* ip) {
	// pointer for the linked list of addrinfo result
	addrinfo* res = get_addr_info(ip, DEFAULT_PORT);
	addrinfo *ipv4 = get_IPv4_info(res);

	// socket file descriptor we are listening to incoming connections
	int listener = get_sockfd(ipv4->ai_family, ipv4->ai_socktype, ipv4->ai_protocol);

	set_socket_reuse(listener);
	bind_socket(listener, ipv4);
	set_backlog(listener, BACKLOG);

	// No longer need addrinfos
	freeaddrinfo(res);

	// Make my listener nonblocking
	fcntl(listener, F_SETFL, O_NONBLOCK);

	return listener;
}

/**
 * Accept new connections
 *
 * @param listener is the file descriptor being listened to
 * @param their_addr is their address as sockaddr_storage
 * @param size is the size of sockaddr_storage
 * @return sockfd of the accepted connection
 */
inline int make_conn_fd(int listener, sockaddr_storage* their_addr, socklen_t size) {
	int new_fd = accept(listener, (struct sockaddr *) their_addr, &size);
	if (new_fd == -1) {
		fprintf(stderr, "Accept error\n");
	}
	return new_fd;
}

/**
 * Connect to default server
 */
inline int connect_to_default_server() {
	addrinfo* res = get_addr_info(DEFAULT_SERVER_IP, DEFAULT_PORT);
	addrinfo *ipv4 = get_IPv4_info(res);
	int server = get_sockfd(ipv4->ai_family, ipv4->ai_socktype, ipv4->ai_protocol);
	connect_to(server, ipv4);
	fcntl(server, F_SETFL, O_NONBLOCK); // only for polling?
	freeaddrinfo(res);
	return server;
}
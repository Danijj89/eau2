#include "network.h"
#include "network2.h"
#include "NodeInfoArray.h"
#include "pollfds.h"

/**
 * Kernode: Represent the minimum capabilities of a node in a network.
 */
class Kernode {
public:
	NodeInfoArray* addressBook_;
	Pollfds* connections_;
	NodeInfo* myInfo_;
	int id_;
	bool running_;

	Kernode(String* ip, int port) {
		this->myInfo_ = new NodeInfo(ip, port, 0);
		this->addressBook_ = new NodeInfoArray();
		this->running_ = true;
	}

	virtual ~Kernode() {
		delete this->myInfo_;
		delete this->addressBook_;
		delete this->connections_;
	}

	virtual int setupListener(String* ip, int port) {
		int listener = get_new_fd();
		bind_fd_to_adr(this->myInfo_->get_ip(), this->myInfo_->get_port(), listener);
		assert (listen(listener, 5) == 0);
		return listener;
	}

	virtual int connectToServer(String* ip, int port) {
		return -1;
	}

	virtual void connectToAllNodes() {}

	virtual void run() {
		String* ip = this->myInfo_->get_ip();
		int port = this->myInfo_->get_port();
		int listener = this->setupListener(ip, port);
		int server = this->connectToServer(ip, port);
		this->connections_ = new Pollfds(listener, server);
		connectToAllNodes();
		loop();
	}

	virtual void loop() {
		char buf [1024] = {0};
		while(this->running_) {
			if (poll(this->connections_->pfds(), this->connections_->size(), -1) == -1) {
				fprintf(stderr, "Error with poll mechanism.\n");
			}

			for (size_t i = 0; i < this->connections_->size(); ++i) {
				if (this->connections_->pfds()[i].revents == POLLIN) {
					printf("KERNODE: found something...\n");
					if (i == 0) { // stdin
						read_from(0, buf, 1024);
						printf("KERNODE: %s", buf);
					} else if (i == 1) { // Listener port
						// Handle new connection
						printf("Got to 1\n");
					} else if (i == 2) { // Server connection
						// Handle new instructions from server
						printf("Got to 2\n");
					} else { // Every other connection
						// Handle requests from other nodes
						printf("Got to other\n");
					}
				}
			}
		}
	}
};

#include <stdio.h>
#include <iostream>
#include <signal.h>


#include "../util/string.h"


#include "kernode.h"
#include "network2.h"

int main(int argc, char** argv) {
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	for (int i = 0; i < NUM_NODES; ++i) {
		String ip("");
		int port;
		switch (i) {
			case 0:
				ip = String(SERVER_IP);
				port = SERVER_PORT;
				break;
			case 1:
				ip = String(NODE1_IP);
				port = PORT1;
				break;
			case 2:
				ip = String(NODE2_IP);
				port = PORT2;
				break;
			default:
				assert(false);
		}
		if (!fork()) { // Child process
			Kernode* node = new Kernode(i, &ip, port);
			node->initialize();
			delete node;
			exit(0);
		} // main process
		sleep(3);
		printf("Started Node %u\n", i);
	}

	return 0;
}
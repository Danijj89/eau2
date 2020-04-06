#include <stdio.h>
#include <iostream>
#include <signal.h>


#include "../util/string.h"


#include "kernode.h"

int main(int argc, char** argv) {
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	for (unsigned int i = 0; i < NUM_NODES + 1; ++i) {
		if (!fork()) { // Child process
			Kernode* node = new Kernode(i);
			node->run();
			delete node;
			exit(0);
		} // main process
		sleep(3);
		printf("Started Node %u\n", i);
	}

	return 0;
}
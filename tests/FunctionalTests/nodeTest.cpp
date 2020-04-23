#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../../src/network/node.h"


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


int main(int argc, char** argv) {
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	String* serverIP = new String("127.0.0.1");
	String* clientIP = new String("127.0.0.2");
	int serverPort = 9000;
	int clientPort = 9001;
	size_t numNodes = 2;
	int socketBacklog = 4;
	char* file = const_cast<char*>("../100k.txt");
	size_t chunkItems = 1000;

	NodeConfiguration** conf = new NodeConfiguration*[numNodes];

	conf[0] = new NodeConfiguration();
	conf[0]->setIP(serverIP);
	conf[0]->setPort(serverPort);
	conf[0]->setNumNodes(numNodes);
	conf[0]->setSocketBacklog(socketBacklog);
	conf[0]->setServerIP(serverIP);
	conf[0]->setServerPort(serverPort);
	conf[0]->setFilePath(file);
	conf[0]->setChunkItems(chunkItems);

	conf[1] = new NodeConfiguration();
	conf[1]->setIP(clientIP);
	conf[1]->setPort(clientPort);
	conf[1]->setNumNodes(numNodes);
	conf[1]->setSocketBacklog(socketBacklog);
	conf[1]->setServerIP(serverIP);
	conf[1]->setServerPort(serverPort);
	conf[1]->setFilePath(file);
	conf[1]->setChunkItems(chunkItems);


	for (unsigned int i = 0; i < numNodes + 1; ++i) {
		if (!fork()) { // Child process
			Node* node = new Node(conf[i]);
			node->shutdown();
			delete node;
			exit(0);
		} // main process
		sleep(3);
		printf("Started Node %u\n", i);
	}

	for (size_t i = 0; i < numNodes; ++i) {
		delete conf[i];
	}
	delete[] conf;

	return 0;
}

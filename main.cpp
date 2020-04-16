

//#include <thread>
//#include "src/application/demo.h"
#include <stdio.h>
#include <signal.h>
#include "src/util/constants.h"
#include "src/util/string.h"
#include "src/network/node.h"
#include "src/network/network2.h"

//void thread_job(Demo* app) {
//	app->run_();
//}


int main() {
//	int numNodes = 3;
//	std::thread** workers = new std::thread*[numNodes];
//	Demo** apps = new Demo*[numNodes];
//	KVStore** stores = new KVStore*[numNodes];
//	for (int i = 0; i < numNodes; i++) {
//		stores[i] = new KVStore(i);
//		apps[i] = new Demo(i, stores[i]);
//	}
//	for (int i = 0; i < numNodes; i++) {
//		stores[i]->setStores(stores, numNodes);
//	}
//	for (int i = 0; i < numNodes; i++) {
//		workers[i] = new std::thread(thread_job, apps[i]);
//	}
//	for (int i = 0; i < numNodes; i++) {
//		workers[i]->join();
//	}
//	delete[] stores;
//	for (int i = 0; i < numNodes; i++) {
//		delete apps[i];
//		delete workers[i];
//	}
//	delete[] workers;
//	delete[] apps;
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	for (int i = 0; i < NUM_NODES; ++i) {
		String* ip;
		int port;
		switch (i) {
			case 0:
				ip = new String(SERVER_IP);
				port = SERVER_PORT;
				break;
			case 1:
				ip = new String(NODE1_IP);
				port = PORT1;
				break;
			case 2:
				ip = new String(NODE2_IP);
				port = PORT2;
				break;
			default:
				assert(false);
		}
		if (!fork()) { // Child process
			Node* node = new Node(i, ip, port);
			node->initialize();
			delete node;
			exit(0);
		} // main process
		sleep(3);
		printf("Started Node %u\n", i);
	}
	return 0;
}



#include <thread>
#include "src/application/demo.h"

void thread_job(Demo* app) {
	app->run_();
}


int main() {
	int numNodes = 3;
	std::thread** workers = new std::thread*[numNodes];
	Demo** apps = new Demo*[numNodes];
	KVStore** stores = new KVStore*[numNodes];
	for (int i = 0; i < numNodes; i++) {
		stores[i] = new KVStore(i);
		apps[i] = new Demo(i, stores[i]);
	}
	for (int i = 0; i < numNodes; i++) {
		stores[i]->setStores(stores, numNodes);
	}
	for (int i = 0; i < numNodes; i++) {
		workers[i] = new std::thread(thread_job, apps[i]);
	}
	for (int i = 0; i < numNodes; i++) {
		workers[i]->join();
	}
	delete[] stores;
	for (int i = 0; i < numNodes; i++) {
		delete apps[i];
		delete workers[i];
	}
	delete[] workers;
	delete[] apps;
	return 0;
}

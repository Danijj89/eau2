

#include "src/application/demo.h"

void thread_job(size_t id, KVStore* store) {
	Demo* app = new Demo(id, store);
	app->run_();
}


int main() {
	int numNodes = 3;
	std::thread** workers = new std::thread*[numNodes];
	KVStore** stores = new KVStore*[numNodes];
	for (int i = 0; i < numNodes; i++) {
		stores[i] = new KVStore(i);
	}
	for (int i = 0; i < numNodes; i++) {
		stores[i]->setStores(stores, numNodes);
	}
	for (int i = 0; i < numNodes; i++) {
		workers[i] = new std::thread(thread_job, i, stores[i]);
	}
	for (int i = 0; i < numNodes; i++) {
		workers[i]->join();
	}
	delete[] stores;
	for (int i = 0; i < numNodes; i++) {
		delete workers[i];
	}
	delete[] workers;
	return 0;
}

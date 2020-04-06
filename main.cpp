

//#include <thread>
//#include "src/application/demo.h"
#include <cstdlib>
#include <unordered_map>
#include "src/kvstore/key.h"
#include "src/kvstore/value.h"

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

	std::unordered_map<Key, Value, KeyHashFunction> store;
	Key* k = new Key("k1", 0);
	char str[MAX_BLOB_SIZE] = {0};
	memcpy(str, const_cast<char*>("123"), 3);
	Value* v = new Value(str, 1);
	store[*k] = *v;
	if (store.find(k) == store.end()) {
		std::cout << "Not found" << '\n';
	} else {
		std::cout << store[k].getBlob() << '\n';
	}
	return 0;
}

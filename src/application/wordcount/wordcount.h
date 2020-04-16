#pragma once

#include "../application.h"
#include "filereader.h"
#include "adder.h"
#include "summer.h"

class WordCount: public Application {
public:
	static const size_t BUFSIZE = 1024;
	Key in;
	Key* keys[NUM_NODES] = {nullptr};
	std::unordered_map<String, int, StringHashFunction> all;

	WordCount(int idx, KVStore* store) : Application(idx, store), in("data") { }

	/** The master nodes reads the input, then all of the nodes count. */
	void run_() override {
		if (this->getNodeId() == 0) {
			FileReader fr("100k.txt");
			delete this->fromVisitor(&in, "S", &fr);
		}
		this->local_count();
		this->reduce();
	}

	/** Returns a key for given node.  These keys are homed on master node
	 *  which then joins them one by one. */
	Key* mk_key(int idx) {
		StrBuff sb = StrBuff();
		sb.c("wc-map-");
		sb.c(idx);

		Key* k = new Key(sb.get(), SERVER_ID);
		printf("Created key %s", k->getKey()->c_str());
		return k;
	}

	/** Compute word counts on the local node and build a data frame. */
	void local_count() {
		DataFrame* words = (this->waitAndGet(&in));
		p("Node ").p(this->getNodeId()).pln(": starting local count...");
		std::unordered_map<String, int, StringHashFunction> map;
		Adder add(map);
		words->localMap(&add);
		delete words;
		Summer cnt(map);
		delete this->fromVisitor(this->mk_key(this->getNodeId()), "SI", &cnt);
	}

	/** Merge the data frames of all nodes */
	void reduce() {
		if (this->getNodeId() != 0) return;
		pln("Node 0: reducing counts...");
		std::unordered_map<String, int, StringHashFunction> map;
		Key* own = mk_key(0);
		merge(this->get(own), map);
		for (int i = 1; i < NUM_NODES; ++i) { // merge other nodes
			Key* ok = mk_key(i);
			merge(this->waitAndGet(ok), map);
			delete ok;
		}
		p("Different words: ").pln(map.size());
		delete own;
	}

	void merge(DataFrame* df, std::unordered_map<String, int, StringHashFunction> m) {
		Adder add(m);
		df->map(add);
		delete df;
	}
}; // WordcountDemo
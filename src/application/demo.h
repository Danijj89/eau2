#pragma once


#include "Application.h"

class Demo : public Application {
public:
	Key main = Key("main");
	Key verify = Key("verif");
	Key check = Key("ck");

	Demo(int idx, KVStore* store): Application(idx, store) {}

	void run_() override {
		switch(this->getNodeId()) {
			case 0:
				producer();
				break;
			case 1:
				counter();
				break;
			case 2:
				summarizer();
				break;
			default:
				;
		}
	}

	void producer() {
		size_t size = 1000*1000;
		double* vals = new double[size];
		double sum = 0;
		for (size_t i = 0; i < size; ++i) sum += vals[i] = i;
		delete this->fromArray(&main, vals, size);
		delete this->fromScalar(&check, sum);
		delete[] vals;
	}

	void counter() {
		DataFrame* v = this->waitAndGet(&main);
		size_t sum = 0;
		for (size_t i = 0; i < 1000*1000; ++i) sum += v->getDouble(0,i);
		p("The sum is  ").pln(sum);
		delete this->fromScalar(&verify, sum);
		delete v;
	}

	void summarizer() {
		DataFrame* result = this->waitAndGet(&verify);
		DataFrame* expected = this->waitAndGet(&check);
		pln(expected->getDouble(0,0)==result->getDouble(0,0) ? "SUCCESS":"FAILURE");
		delete result;
		delete expected;
	}
};
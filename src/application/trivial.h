#pragma once

#include "Application.h"
#include "../dataframe/dataframe.h"

class Trivial : public Application {
public:

	explicit Trivial(int idx) : Application(idx) {}

	void run_() override {
		size_t SZ = 1000* 1000;
		double* vals = new double[SZ];
		double sum = 0;
		for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
		Key k = Key("triv",0);
		DataFrame* df = this->fromArray(&k, vals, SZ);
		assert(df->getDouble(0,1) == 1);
		DataFrame* df2 = this->get(&k);
		for (size_t i = 0; i < SZ; ++i) sum -= df2->getDouble(0,i);
		assert(sum==0);
		delete[] vals;
		delete df;
		delete df2;
	}
};

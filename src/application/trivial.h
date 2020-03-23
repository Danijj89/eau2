#pragma once

#include "Application.h"
#include "../dataframe/dataframe.h"

class Trivial : public Application {
public:

	Trivial(int idx) : Application(idx, new String(APPLICATION_IP), APPLICATION_PORT) {}

	void run_() override {
		size_t SZ = 1000;
		float* vals = new float[SZ];
		float sum = 0;
		for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
		Key k = Key("triv",0);
		DataFrame* df = this->fromArray(&k, vals, SZ);
		assert(df->get_float(0,1) == 1);
		DataFrame* df2 = this->get(&k);
		for (size_t i = 0; i < SZ; ++i) sum -= df2->get_float(0,i);
		assert(sum==0);
		delete df;
		delete df2;
	}
};

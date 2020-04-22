#pragma once


#include "../dataframe/rower.h"

class Reader : public Rower {
public:

	bool accept(Row& r) override {
		this->visit(r);
		return true;
	}

	virtual bool visit(Row& r) = 0;

	virtual bool done() { return false; }
};

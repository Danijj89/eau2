#pragma once


#include "../dataframe/rower.h"

class Writer : public Rower {
public:

	bool accept(Row& r) override {
		this->visit(r);
		return true;
	}

	virtual void visit(Row& r) = 0;

	virtual bool done() { return false; }
};
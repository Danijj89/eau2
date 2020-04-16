#pragma once

#include "../../dataframe/row.h"

class Writer : public Rower {
public:

	bool accept(Row& r) override {
		this->visit(r);
	}

	virtual void visit(Row& r) = 0;

	virtual bool done() { return false; }
};
#pragma once

#include "../../dataframe/row.h"

class ColumnWriter : public Object {
public:
	size_t nRows = 0;
	int n = 0;

	virtual void visit(Column** cols, Row* r) {
		for (size_t i = 0; i < r->width(), ++i) {
			switch (r->colType(i)) {
				case 'B':
					cols[i]->pushBack(r->getBool(i));
					break;
				case 'I':
					cols[i]->pushBack(r->getInt(i));
					break;
				case 'D':
					cols[i]->pushBack(r->getDouble(i));
					break;
				case 'S':
					cols[i]->pushBack(r->getString(i));
					break;
				default:
					assert(false);
			}
		}

		this.nRows += 1;
		if (this.nRows % CACHE_ELEMENT_SIZE == 0) {
			for (size_t i = 0; i < r->width(), ++i) {
				cols[i]->dumpCacheToNode(n % NUM_NODES + 1);
			}
		}
		this.n += 1;
	}

	virtual bool done() { return false; }
};
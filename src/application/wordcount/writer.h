

#include "../../dataframe/row.h"

class Visitor {
public:

	virtual void visit(Row& r) = 0;

	virtual bool done() { return false; }
};
//lang:Cpp
#pragma once

#include <unordered_map>
#include "../../util/string.h"
#include "writer.h"

class Summer : public Writer {
public:
	std::unordered_map<String, int, StringHashFunction> map_;
	std::unordered_map<String, int, StringHashFunction>::iterator itr;
	size_t seen = 0;

	Summer(std::unordered_map<String, int, StringHashFunction>& map) : map_(map) {}

	void visit(Row& r) {
		r.set(0, const_cast<String*>(&itr->first));
		r.set(1, itr->second);
		itr++;
	}

	bool done() {return itr == this->map_.end(); }
};
//lang:Cpp
#pragma once

#include <unordered_map>
#include "../writer.h"


class Summer : public Writer {
public:
	std::unordered_map<String, int, StringHashFunction>* map_;
	std::unordered_map<String, int, StringHashFunction>::iterator itr;

	Summer(std::unordered_map<String, int, StringHashFunction>* map) : map_(map) {
		this->itr = this->map_->begin();
	}

	void visit(Row& r) {
		r.set(0, const_cast<String*>(&itr->first));
		r.set(1, itr->second);
		itr++;
	}

	bool done() {return itr == this->map_->end(); }
};
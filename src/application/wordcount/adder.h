#pragma once

#include <unordered_map>
#include "../reader.h"


class Adder : public Reader {
public:
	std::unordered_map<String, int, StringHashFunction>* map_;

	Adder(std::unordered_map<String, int, StringHashFunction>* map) : map_(map) {}

	bool visit(Row& r) override {
		String* word = r.getString(0);
		assert(word != nullptr);
		int num = this->map_->find(*word) == this->map_->end() ? 0 : (*this->map_)[*word];
		num++;
		(*this->map_)[*word] = num;
		return true;
	}
};
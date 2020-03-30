#pragma once

#include "node_info.h"
#include "../util/array.h"

class NodeInfoArray : public Array {

public:
	NodeInfo** vals_; // owned

	NodeInfoArray() {
		this->vals_ = new NodeInfo*[this->capacity_];
	}

	virtual ~NodeInfoArray() {
		for (size_t i = 0; i < this->size_; ++i) {
			delete this->vals_[i];
		}
		delete[] this->vals_;
	}

	NodeInfo* get(size_t i) {
		this->assertIndexInBounds(i);
		return this->vals_[i];
	}

	void set(size_t i, NodeInfo* val) {
		this->assertIndexInBounds(i);
		this->vals_[i] = val;
	}

	/**
	 * Grows the array if it has reached it's capacity limit.
	 */
	void resize() override {
		this->capacity_ *= 2;
		NodeInfo** new_vals = new NodeInfo*[this->capacity_];
		for (size_t i = 0; i < this->size_; ++i) {
			new_vals[i] = this->vals_[i];
		}
		delete[] this->vals_;
		this->vals_ = new_vals;
	}

	/**
	 * Pushes the given item to the end of this array.
	 * NOTE: nullptr are valid values
	 *
	 * @param item the given item to be added to the end of this array
	 */
	void pushBack(NodeInfo* s) {
		this->resizeIfFull();
		this->vals_[this->size_] = s;
		this->size_++;
	}

	String* toString() {
		StrBuff* sb = new StrBuff();
		for (size_t i = 0; i < this->size_; ++i) {
			sb->c(this->vals_[i]->to_string()->c_str());
			sb->c("\n");
		}
		String* s = sb->get();
		delete sb;
		return s;
	}

	NodeInfo* find(NodeInfo* n) {
		for (size_t i = 0; i < this->size_; ++i) {
			if (this->vals_[i]->get_ip()->equals(n->get_ip())
			&& this->vals_[i]->get_port() == n->get_port()) {
				return this->vals_[i];
			}
		}
		return nullptr;
	}
};
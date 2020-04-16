#pragma once

#include "../util/object.h"
#include "DFData.h"

class DFArray : public Object {
public:
	DFData* vals_;
	size_t size_;
	size_t capacity_;

	DFArray() {
		this->size_ = 0;
		this->capacity_ = 4;
		this->vals_ = new DFData[this->capacity_];
	}

	DFArray(size_t size) {
		this->size_ = 0;
		this->capacity_ = size;
		this->vals_ = new DFData[this->capacity_];
	}

	~DFArray() { delete[] this->vals_; }

	size_t size() { return this->size_; }

	void set(size_t idx, DFData v) {
		assert(idx < this->size_);
		this->vals_[idx] = v;
	}

	DFData get(size_t idx) {
		assert(idx < this->size_);
		return this->vals_[idx];
	}

	void pushBack(DFData v) {
		if (this->size_ == this->capacity_) this->resize_();
		this->vals_[this->size_] = v;
		this->size_++;
	}

	void resize_() {
		DFData* temp = new DFData[this->capacity_ * 2];
		for (size_t i = 0; i < this->size_; i++) {
			temp[i] = this->vals_[i];
		}
		delete this->vals_;
		this->vals_ = temp;
	}

	DFData* getData() {
		return this->vals_;
	}
};
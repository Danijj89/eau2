#pragma once


#include "../util/object.h"
#include "df_data.h"
#include "../util/array.h"

class DFDataArray : public Object {
public:
	Array* array_;

	DFDataArray() {
		this->array_ = new Array();
	}

	DFDataArray(size_t size) {
		this->array_ = new Array(size);
	}

	~DFDataArray() { delete this->array_; }

	size_t size() { return this->array_->size(); }

	void set(size_t idx, DFData* v) {
		this->array_->set(idx, v);
	}

	DFData* get(size_t idx) {
		DFData* v = dynamic_cast<DFData*>(this->array_->get(idx));
		assert(v != nullptr);
		return v;
	}

	void pushBack(DFData* v) {
		this->array_->pushBack(v);
	}

	String* serialize(char type) {
		StrBuff sb = StrBuff();
		Serializer s = Serializer();
		s.serializeSizeT(this->size());
		sb.c(*s.get());
		for (size_t i = 0; i < this->size(); i++) {
			sb.c(*this->get(i)->serialize(type));
		}
		return sb.get();
	}

	static DFDataArray* deserialize(char* buff, size_t* counter, char type) {
		DFDataArray* result = new DFDataArray();
		Deserializer d = Deserializer();
		size_t size = d.deserializeSizeT(buff, counter);
		for (size_t i = 0; i < size; i++) {
			result->pushBack(DFData::deserialize(buff, counter, type));
		}
		return result;
	}
};
#pragma once

#include "../util/object.h"
#include "../dataframe/dataframe.h"
#include "../kvstore/KVStore.h"


class Application : public Object {
public:
	int id_;
	KVStore* store_;
	Serializer* ser_;
	Deserializer* des_;

	Application(int id) {
		this->id_ = id;
		this->store_ = new KVStore(this->id_);
		this->ser_ = new Serializer();
		this->des_ = new Deserializer();
	}

	~Application() override {
		delete this->store_;
		delete this->ser_;
		delete this->des_;
	}

	virtual void run_() {}

	DataFrame* fromArray(Key* k, float* vals, size_t n) {
		DataFrame* result = new DataFrame();
		Column* col = new FloatColumn(getId(k->getKey(), 0), this->store_);
		size_t chunks = n / MAX_FLOAT_ELEMENTS;
		size_t last_chuck_size = n % MAX_FLOAT_ELEMENTS;
		// pushing all full size chunks
		for (size_t i = 0; i < chunks; ++i) {
			col->pushBack(vals + i * MAX_FLOAT_ELEMENTS, MAX_FLOAT_ELEMENTS);
		}
		// pushing last chunk that might not be full sized
		col->pushBack(vals + chunks * MAX_FLOAT_ELEMENTS, last_chuck_size);
		result->add_column(col);
		this->ser_->serialize_column(col);
		Value* v = new Value(this->ser_->get_buff(), col->getKeys()->len());
		this->store_->put(k, v);
		return result;
	}

	DataFrame* get(Key* k) {
		Value* v = this->store_->get(k);
		Column* col = this->des_->deserialize_column(v->getBlob(), v->getSize(), this->store_);
		DataFrame* result = new DataFrame();
		result->add_column(col);
		return result;
	}

};
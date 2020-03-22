#pragma once

#include "../util/object.h"
#include "../kvstore/KVStore.h"
#include "../dataframe/dataframe.h"

class Application : public Object {
public:
	int id_;
	KVStore* store_;
	Serializer ser_;
	Deserializer des_;

	Application(int id, String* ip, int port) {
		this->id_ = id;
		this->store_ = new KVStore(this->id_, ip, port);
	}

	~Application() override {
		delete this->store_;
	}

	virtual void run_() = 0;

	DataFrame* fromFloatArray(Key* k, float* vals, size_t n) {
		DataFrame* result = new DataFrame();
		FloatColumn* col = new FloatColumn();
		size_t chunks = n / MAX_FLOAT_ELEMENTS;
		size_t last_chuck_size = n % MAX_FLOAT_ELEMENTS;
		// pushing all full size chunks
		for (size_t i = 0; i < chunks; ++i) {
			col->pushBack(vals + i * MAX_FLOAT_ELEMENTS, MAX_FLOAT_ELEMENTS);
		}
		// pushing last chunk that might not be full sized
		col->pushBack(vals + chunks * MAX_FLOAT_ELEMENTS, last_chuck_size);
		result->add_column(col);
		Value* v = new Value(this->ser_.serialize_column(col), col->getKeys()->len());
		this->store_->put(k, v);
		return result;
	}

};
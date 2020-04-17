#pragma once

#include "../util/object.h"
#include "../dataframe/dataframe.h"
#include "../kvstore/KVStore.h"
#include "wordcount/writer.h"
#include "../dataframe/df_array.h"


class Application : public Object {
public:
	int id_;
	KVStore* store_;

	Application(int id, KVStore* store) {
		this->id_ = id;
		this->store_ = store;
	}

	~Application() override {
		delete this->store_;
	}

	virtual void run_() {}

	DataFrame* get(Key* k) {
		Value* v = this->store_->get(k);
		Deserializer des = Deserializer();
		DataFrame* result = des.deserialize_dataframe(v->getBlob(), this->store_);
		return result;
	}

	DataFrame* waitAndGet(Key* k) {
		Value* v = this->store_->waitAndGet(k);
		Deserializer des = Deserializer();
		DataFrame* result = des.deserialize_dataframe(v->getBlob(), this->store_);
		return result;
	}

	int getNodeId() {
		return this->id_;
	}

	DataFrame* fromVisitor(Key* k, const char* types, Writer* w) {
		size_t nCols = strlen(types);
		Schema s = Schema(types, nCols);
		Row r = Row(&s);
		Column** cols = makeColumnar(&s);

		ColumnarWriter cw = new ColumnarWriter(&s);
		while(!w->done()) {
			w->visit(r);
			cw->visit(cols, &r); //
		}

		DataFrame* result = new DataFrame();
		results->addColumns(cols, nCols);

		Serializer s = Serializer();
		s.serialize_dataframe(result);
		Value* v = new Value();
		v.setBlob(s.get_buff());
		this->store_->put(k, v);
		return result;
	}
};
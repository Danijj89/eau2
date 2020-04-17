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
		DataFrame* result = new DataFrame();
		size_t nCols = strlen(types);
		Column** cols = new Column*[nCols];

		DFArray** temp = new DFArray*[nCols];
		Schema s = Schema(types, nCols);
		Row r = Row(&s);
		size_t nodeId = 0;

		while (!w->done()) {
			for (size_t i = 0; i < CHUNK_ELEMENTS && !w->done(); i++) {
				w->visit(r);
				for (size_t c = 0; c < nCols; c++) {
					DFData v = DFData();
					switch (types[c]) {
						case 'B':
							v.payload_.b = r.getBool(c);
							temp[c]->pushBack(v);
							break;
						case 'I':
							v.payload_.i = r.getInt(c);
							temp[c]->pushBack(v);
							break;
						case 'D':
							v.payload_.d = r.getDouble(c);
							temp[c]->pushBack(v);
							break;
						case 'S':
							v.payload_.s = r.getString(c);
							temp[c]->pushBack(v);
							break;
						default:
							assert(false);
					}
				}
			}
			for (size_t i = 0; i < nCols; i++) {
				switch (types[i]) {
					case 'B':
						cols[i]->pushBack(temp[i]->getAsBools(), temp[i]->size(), nodeId);
						break;
					case 'I':
						cols[i]->pushBack(temp[i]->getAsInts(), temp[i]->size(), nodeId);
						break;
					case 'D':
						cols[i]->pushBack(temp[i]->getAsDoubles(), temp[i]->size(), nodeId);;
						break;
					case 'S':
						cols[i]->pushBack(temp[i]->getAsStrings(), temp[i]->size(), nodeId);
						break;
					default:
						assert(false);
				}
			}
			nodeId = nodeId == NUM_NODES - 1 ? 0 : nodeId + 1;
		}

		for (size_t i = 0; i < nCols; i++) {
			result->addColumn(cols[i]);
		}

		Serializer ser = Serializer();
		ser.serialize_dataframe(result);
		Value* v = new Value(ser.get_buff(), nCols);
		this->store_->put(k, v);

		return result;
	}

};
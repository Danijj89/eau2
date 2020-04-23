#pragma once


#include "../util/object.h"
#include "../dataframe/distributed_columns.h"
#include "../kvstore/KVStore.h"
#include "writer.h"
#include "../dataframe/dataframe.h"
#include "../network/node_configuration.h"

class Application : public Object {
public:
	KVStore* store_; // owned
	size_t numNodes_;
	size_t chunkItems_;


	Application(NodeConfiguration* conf) {
		this->store_ = new KVStore(conf);
		this->numNodes_ = conf->getNumNodes();
		this->chunkItems_ = conf->getChunkItems();
	}

	~Application() override {
		delete this->store_;
	}

	virtual void run() = 0;

	size_t getNodeId() { return this->store_->getNodeId(); }

	size_t getNumNodes() { return this->numNodes_; }

	DataFrame* get(Key* k) {
		Value* v = this->store_->getUserData(k);
		assert(v != nullptr);
		size_t counter = 0;
		return DataFrame::deserializeDataFrame(v->getBlob(), &counter, this->store_);
	}

	DataFrame* waitAndGet(Key* k) {
		Value* v = this->store_->waitAndGetUserData(k);
		assert(v != nullptr);
		size_t counter = 0;
		return DataFrame::deserializeDataFrame(v->getBlob(), &counter, this->store_);
	}

	DataFrame* fromFile(Key* k, const char* filePath) { return nullptr; }

	DataFrame* fromScalar(Key* k, int v) { return nullptr; }

	DataFrame* fromVisitor(Key* k, const char* types, Writer* w) {
		Schema* schema = new Schema(types);
		DistributedColumns* cols = new DistributedColumns(schema, this->store_);
		Row r = Row(cols->getSchema());
		size_t nCols = schema->width();

		size_t keyIdx = 0;
		size_t lastChunkSize = 0;
		while (!w->done()) {
			DFDataArray** temp = this->fillTempChunk_(&r, nCols, w, types);
			for (size_t col = 0; col < nCols; col++) {
				Key* curr = this->makeKey_(k->getKey()->c_str(), col, keyIdx, keyIdx % this->numNodes_);
				cols->addKey(col, new Key(curr));
				this->store_->put(curr, new Value(temp[col]->serialize(types[col]))); // takes ownership of both Key and Value
			}
			keyIdx++;
			lastChunkSize = temp[0]->size();
			for (size_t i = 0; i < nCols; i++) {
				delete temp[i];
			}
			delete[] temp;
		}

		cols->setLastChunkSize(lastChunkSize);

		DataFrame* result = new DataFrame(cols);
		this->store_->putUserData(new Key(k), new Value(result->serialize())); // takes ownership of both Key and Value
		return result;
	}

	DFDataArray** fillTempChunk_(Row* r, size_t nCols, Writer* w, const char* types) {
		DFDataArray** temp = new DFDataArray*[nCols];
		for (size_t i = 0; i < nCols; i++) {
			temp[i] = new DFDataArray();
		}
		for (size_t row = 0; row < this->chunkItems_ && !w->done(); row++) {
			w->visit(*r);
			for (size_t col = 0; col < nCols; col++) {
				temp[col]->pushBack(new DFData(r->getDFData(col), types[col]));
			}
		}
		return temp;
	}

	Key* makeKey_(char* prefix, size_t col, size_t row, size_t nodeId) {
		StrBuff s = StrBuff();
		s.c(prefix);
		s.c("-");
		s.c(col);
		s.c("-");
		s.c(row);
		return new Key(s.get(), nodeId);
	}

	void waitShutDown() {
		while (this->store_->running_) {
			sleep(3);
		}
	}

	void shutdown() {
		this->store_->shutdown();
	}
};
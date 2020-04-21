#pragma once


#include "../util/object.h"
#include "schema.h"
#include "../kvstore/key_array.h"
#include "df_data.h"
#include "../util/constants.h"
#include "../kvstore/KVStore.h"
#include "../kvstore/value.h"
#include "df_data_array.h"
#include "rower.h"


class DistributedColumns : public Object {
public:
	Schema* schema_; // owned
	size_t width_;
	KeyArray** keys_; // owned
	size_t lastChunkSize_;
	KVStore* store_; // external
	Key* cacheKey_; // external (point to keys in the key's arrays)
	DFDataArray* cacheValue_; // owned
	size_t chunkItems_;

	DistributedColumns(Schema* schema, KVStore* store) {
		this->schema_ = schema;
		this->width_ = this->schema_->width();
		this->keys_ = new KeyArray*[this->width_];
		for (size_t i = 0; i < this->width_; i++) {
			this->keys_[i] = new KeyArray();
		}
		this->lastChunkSize_ = 0;
		this->store_ = store;
		this->cacheKey_ = nullptr;
		this->cacheValue_ = nullptr;
		this->chunkItems_ = store->getChunkItems();
	}

	DistributedColumns(Schema* schema, KeyArray** keys, size_t lastChunkSize, KVStore* store) {
		this->schema_ = schema;
		this->width_ = this->schema_->width();
		this->keys_ = keys;
		this->lastChunkSize_ = lastChunkSize;
		this->store_ = store;
		this->cacheKey_ = nullptr;
		this->cacheValue_ = nullptr;
	}

	~DistributedColumns() {
		delete this->schema_;
		for (size_t i = 0; i < this->width_; i++) {
			delete this->keys_[i];
		}
		delete[] this->keys_;
		delete this->cacheValue_;
	}

	Schema* getSchema() {
		return this->schema_;
	}

	void addKey(size_t col, Key* k) {
		assert(col < this->width_);
		this->keys_[col]->pushBack(k);
	}

	bool getBool(size_t col, size_t row) {
		this->assertValidIndices_(col, row, 'B');
		size_t colIdx = this->getKeyIdx_(row);
		size_t rowIdx = this->getChunkIdx_(row);
		Key* k = this->keys_[col]->get(colIdx);
		// Check cache first
		if (this->cacheKey_->equals(k)) return this->cacheValue_->get(rowIdx)->payload_.b;
		// Get value from store and cache it
		this->addStoreValueToLocalCache_(k, this->schema_->colType(col));
		return this->cacheValue_->get(rowIdx)->payload_.b;
	}

	int getInt(size_t col, size_t row) {
		this->assertValidIndices_(col, row, 'I');
		size_t colIdx = this->getKeyIdx_(row);
		size_t rowIdx = this->getChunkIdx_(row);
		Key* k = this->keys_[col]->get(colIdx);
		// Check cache first
		if (this->cacheKey_->equals(k)) return this->cacheValue_->get(rowIdx)->payload_.i;
		// Get value from store and cache it
		this->addStoreValueToLocalCache_(k, this->schema_->colType(col));
		return this->cacheValue_->get(rowIdx)->payload_.i;
	}

	double getDouble(size_t col, size_t row) {
		this->assertValidIndices_(col, row, 'D');
		size_t colIdx = this->getKeyIdx_(row);
		size_t rowIdx = this->getChunkIdx_(row);
		Key* k = this->keys_[col]->get(colIdx);
		// Check cache first
		if (this->cacheKey_->equals(k)) return this->cacheValue_->get(rowIdx)->payload_.d;
		// Get value from store and cache it
		this->addStoreValueToLocalCache_(k, this->schema_->colType(col));
		return this->cacheValue_->get(rowIdx)->payload_.d;
	}

	String* getString(size_t col, size_t row) {
		this->assertValidIndices_(col, row, 'S');
		size_t colIdx = this->getKeyIdx_(row);
		size_t rowIdx = this->getChunkIdx_(row);
		Key* k = this->keys_[col]->get(colIdx);
		// Check cache first
		if (k->equals(this->cacheKey_)) return this->cacheValue_->get(rowIdx)->payload_.s;
		// Get value from store and cache it
		this->addStoreValueToLocalCache_(k, this->schema_->colType(col));
		return this->cacheValue_->get(rowIdx)->payload_.s;
	}

	void addStoreValueToLocalCache_(Key* k, char type) {
		Value* v = this->store_->get(k);
		assert(v != nullptr);
		this->addToLocalCache_(k, v, type);
	}

	void addToLocalCache_(Key* k, Value* v, char type) {
		this->cacheKey_ = k;
		delete this->cacheValue_;
		size_t counter = 0;
		this->cacheValue_ = DFDataArray::deserialize(v->getBlob(), &counter, type);
	}

	void assertValidIndices_(size_t col, size_t row, char colType) {
		assert(col < this->width_);
		assert(this->schema_->colType(col) == colType);
		size_t keyIdx = this->getKeyIdx_(row);
		assert(keyIdx < this->keys_[col]->size());
	}

	size_t getKeyIdx_(size_t row) {
		return row / this->chunkItems_;
	}

	size_t getChunkIdx_(size_t row) {
		return row % this->chunkItems_;
	}

	size_t getNumCols() {
		return this->schema_->width();
	}

	size_t getNumRows() {
		return (this->keys_[0]->size() - 1) * this->chunkItems_ + this->lastChunkSize_;
	}

	void setLastChunkSize(size_t n) {
		this->lastChunkSize_ = n;
	}

	void localMap(Rower* v) {
		Row r = Row(this->schema_);
		size_t nRows = this->getNumRows();
		for (size_t row = 0; row < nRows; row++) {
			for (size_t col = 0; col < this->width_; col++) {
				size_t keyIdx = this->getKeyIdx_(row);
				if (this->keys_[col]->get(keyIdx)->getNodeId() == this->store_->getNodeId()) {
					switch (this->schema_->colType(col)) {
						case 'B':
							r.set(col, this->getBool(col, row));
							break;
						case 'I':
							r.set(col, this->getInt(col, row));
							break;
						case 'D':
							r.set(col, this->getDouble(col, row));
							break;
						case 'S':
							r.set(col, this->getString(col, row));
							break;
						default:
							assert(false);
					}
				} else {
					// we expect the entire row to be on the same node, if not then remove this break
					break;
				}
				v->accept(r);
			}
		}
	}

	String* serialize() {
		Serializer s = Serializer();
		StrBuff sb = StrBuff();
		sb.c(*this->schema_->serialize());
		for (size_t i = 0; i < this->width_; i++) {
			sb.c(*this->keys_[i]->serialize());
		}
		sb.c(*s.serializeSizeT(this->lastChunkSize_)->get());
		return sb.get();
	}

	static DistributedColumns* deserialize(char* buff, size_t* counter, KVStore* store) {
		Deserializer d = Deserializer();
		Schema* schema = Schema::deserialize(buff, counter);
		size_t nCols = schema->width();
		KeyArray** keys = new KeyArray*[nCols];
		for (size_t i = 0; i < nCols; i++) {
			keys[i] = KeyArray::deserialize(buff, counter);
		}
		size_t lastChunkSize = d.deserializeSizeT(buff, counter);
		return new DistributedColumns(schema, keys, lastChunkSize, store);
	}
};
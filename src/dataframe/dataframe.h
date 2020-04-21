
#pragma once


#include "../util/object.h"
#include "schema.h"
#include "distributed_columns.h"
#include "row.h"
#include "rower.h"
#include "../application/reader.h"

class DataFrame : public Object {
public:
	Schema* schema_; // external (owned by the distribuited columns)
	DistributedColumns* cols_; // owned

	DataFrame(DistributedColumns* cols) {
		this->schema_ = cols->getSchema();
		this->cols_ = cols;
	}

	~DataFrame() {
		delete this->cols_;
	}

	bool getBool(size_t col, size_t row) {
		return this->cols_->getBool(col, row);
	}

	int getInt(size_t col, size_t row) {
		return this->cols_->getInt(col, row);
	}

	double getDouble(size_t col, size_t row) {
		return this->cols_->getDouble(col, row);
	}

	String*  getString(size_t col, size_t row) {
		return this->cols_->getString(col, row);
	}


	void fillRow(size_t idx, Row* row) {
		for (size_t i = 0; i < this->schema_->width(); ++i) {
			switch(this->schema_->colType(i)) {
				case 'B':
					row->set(i, this->cols_->getBool(i, idx));
					break;
				case 'I':
					row->set(i, this->cols_->getInt(i, idx));
					break;
				case 'D':
					row->set(i, this->cols_->getDouble(i, idx));
					break;
				case 'S':
					row->set(i, this->cols_->getString(i, idx));
					break;
				default:
					exit(1);
			}
		}
	}

	size_t nCols() {
		return this->cols_->getNumCols();
	}

	size_t nRows() {
		return this->cols_->getNumRows();
	}

	void map(Rower* r) {
		Row* row = new Row(this->schema_);
		for (size_t i = 0; i < this->nRows(); ++i) {
			fillRow(i, row);
			r->accept(*row);
		}
	}

	void localMap(Reader* r) {
		this->cols_->localMap(r);
		r->done();
	}

	String* serialize() {
		return this->cols_->serialize();
	}

	static DataFrame* deserializeDataFrame(char* buff, size_t* counter, KVStore* store) {
		return  new DataFrame(DistributedColumns::deserialize(buff, counter, store));
	}
};
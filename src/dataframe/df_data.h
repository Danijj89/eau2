#pragma once

#include "../util/object.h"
#include "../util/string.h"
#include "../serialization/serializer.h"
#include "../serialization/deserializer.h"

union Payload {
	bool b;
	int i;
	double d;
	String* s;
};

class DFData : public Object {
public:
	bool isString_;
	Payload payload_;

	DFData(bool v) {
		this->isString_ = false;
		this->payload_.b =v;
	}

	DFData(int v) {
		this->isString_ = false;
		this->payload_.i = v;
	}

	DFData(double v) {
		this->isString_ = false;
		this->payload_.d = v;
	}

	DFData(String* v) {
		this->isString_ = true;
		this->payload_.s = v;
	}



	DFData(DFData* o, char type) {
		this->isString_ = o->isString_;
		switch (type) {
			case 'B':
				this->payload_.b = o->payload_.b;
				break;
			case 'I':
				this->payload_.i = o->payload_.i;
				break;
			case 'D':
				this->payload_.d = o->payload_.d;
				break;
			case 'S':
				this->payload_.s = new String(*o->payload_.s);
				break;
			default:
				assert(false);
		}
	}

	~DFData() {
		if (this->isString_) {
			delete this->payload_.s;
		}
	}

	String* serialize(char type) {
		Serializer s = Serializer();
		switch (type) {
			case 'B':
				s.serializeBool(this->payload_.b);
				break;
			case 'I':
				s.serializeInt(this->payload_.i);
				break;
			case 'D':
				s.serializeDouble(this->payload_.d);
				break;
			case 'S':
				s.serializeString(this->payload_.s);
				break;
			default:
				assert(false);
		}
		return s.get();
	}

	static DFData* deserialize(char* buff, size_t* counter, char type) {
		DFData* result;
		Deserializer d = Deserializer();
		switch (type) {
			case 'B':
				result = new DFData(d.deserializeBool(buff, counter));
				break;
			case 'I':
				result = new DFData(d.deserializeInt(buff, counter));
				break;
			case 'D':
				result = new DFData(d.deserializeDouble(buff, counter));
				break;
			case 'S':
				result = new DFData(d.deserializeString(buff, counter));
				break;
			default:
				assert(false);
		}
		return result;
	}
};
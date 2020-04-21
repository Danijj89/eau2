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
	bool missing_;
	Payload payload_;

	DFData() {}

	DFData(DFData* o, char type) {
		this->missing_ = o->missing_;
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

	String* serialize(char type) {
		Serializer s = Serializer();
		s.serializeBool(this->missing_);
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
		DFData* result = new DFData();
		Deserializer d = Deserializer();
		bool missing = d.deserializeBool(buff, counter);
		result->missing_ = missing;
		switch (type) {
			case 'B':
				result->payload_.b = d.deserializeBool(buff, counter);
				break;
			case 'I':
				result->payload_.i = d.deserializeInt(buff, counter);
				break;
			case 'D':
				result->payload_.d = d.deserializeDouble(buff, counter);
				break;
			case 'S':
				result->payload_.s = d.deserializeString(buff, counter);
				break;
			default:
				assert(false);
		}
		return result;
	}
};
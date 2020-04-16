#pragma once
#include "../util/string.h"

union Payload {
	bool b;
	int i;
	double d;
	String* s;
};
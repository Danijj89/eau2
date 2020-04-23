#include <string.h>


#include "../../src/kvstore/value.h"


void valueConstructorGetter() {
	char* v = const_cast<char*>("This is a blob.");

	Value value1 = Value(new String(v));

	Value* value2 = new Value(new String(v));

	CS4500_ASSERT_TRUE(value1.getSize() == 1);
	CS4500_ASSERT_TRUE(value2->getSize() == 2);
	CS4500_ASSERT_TRUE(strcmp(value1.getBlob(), value2->getBlob()) == 0);
	CS4500_ASSERT_TRUE(strcmp(value1.getBlob(), v) == 0);

	exit(0);
}


TEST(ValueTests, valueConstructorGetter) {ASSERT_EXIT_ZERO(valueConstructorGetter)}

void valueSetter() {
	char* v = const_cast<char*>("This is a blob.");
	char* new_v = const_cast<char*>("This is another blob.");

	Value value1 = Value(new String(v));

	Value* value2 = new Value(new String(new_v));

	CS4500_ASSERT_TRUE(strcmp(value1.getBlob(), value2->getBlob()) != 0);

	// value1.setBlob(new_v);

	CS4500_ASSERT_TRUE(strcmp(value1.getBlob(), value2->getBlob()) == 0);

	delete value2;

	exit(0);
}

TEST(ValueTests, valueConstructor) {ASSERT_EXIT_ZERO(valueSetter)}




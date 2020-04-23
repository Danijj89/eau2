#include <string.h>


#include "../../src/kvstore/value.h"


void valueConstructorGetter() {
	String* s1 = new String("This is a blob.");
	String* s2 = new String("This is another blob.");

	Value value1 = Value(s1);

	Value* value2 = new Value(s2);

	CS4500_ASSERT_TRUE(value1.getSize() == s1->size());
	CS4500_ASSERT_TRUE(value2->getSize() == s2->size());
	CS4500_ASSERT_TRUE(strcmp(value1.getBlob(), s1->c_str()) == 0);
	CS4500_ASSERT_TRUE(strcmp(value2->getBlob(), s2->c_str()) == 0);

	delete value2;

	exit(0);
}


TEST(ValueTests, valueConstructorGetter) {ASSERT_EXIT_ZERO(valueConstructorGetter)}

void valueSerializeDeserializeTest() {
	String* s = new String("This is a blob");
	Value value1 = Value(s);

	String* serializedValue1 = value1.serialize();

	size_t counter = 0;
	Value* value2 = Value::deserialize(serializedValue1->c_str(), s->size(), &counter);

	ASSERT_TRUE(strcmp(value1.getBlob(), value2->getBlob()) == 0);
	ASSERT_TRUE(value1.getSize() == value2->getSize());

	delete s;
	delete value2;
	delete serializedValue1;

	exit(0);
}

TEST(ValueTests, valueSerializeDeserializeTest) {ASSERT_EXIT_ZERO(valueSerializeDeserializeTest)}




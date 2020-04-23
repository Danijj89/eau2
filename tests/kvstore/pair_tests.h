#include "../../src/kvstore/pair.h"


void pairConstructorAndGetterTest() {
	Key k1 = Key("key1");
	Key* k2 = new Key("key2");

	String s1 = String("This is a blob.");
	String* s2 = new String("This is another blob.");

	Value v1 = Value(&s1);
	Value* v2 = new Value(s2);

	Pair p1 = Pair(&k1, &v1);
	Pair* p2 = new Pair(k2, v2);

	ASSERT_TRUE(strcmp(p1.getKey()->getKey()->c_str(), k1.getKey()->c_str()) == 0);
	ASSERT_TRUE(p2->getValue() == v2);

	delete p2;

	exit(0);
}

TEST(PairTests, pairConstructorAndGetterTest) {ASSERT_EXIT_ZERO(pairConstructorAndGetterTest)}


void pairSerializeDeserializeTest() {
	Key k1 = Key("key1");
	String s1 = String("This is a blob.");
	Value v1 = Value(&s1);

	Pair p1 = Pair(&k1, &v1);

	String* serializedPair = p1.serialize();

	size_t counter = 0;
	Pair* p2 = Pair::deserialize(serializedPair->c_str(), s1.size(), &counter);

	ASSERT_TRUE(strcmp(p1.getKey()->getKey()->c_str(), k1.getKey()->c_str()) == 0);

	ASSERT_TRUE(p1.getValue() == &v1);

	exit(0);
}

TEST(PairTests, pairSerializeDeserializeTest) {ASSERT_EXIT_ZERO(pairSerializeDeserializeTest)}
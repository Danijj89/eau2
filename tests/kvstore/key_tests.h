#include <gtest/gtest.h>


#include <string.h>


#include "../../src/kvstore/key.h"


#define CS4500_ASSERT_TRUE(a)  \
	ASSERT_EQ((a),true);


void keyConstructor() {
	const char* name = "test";
	Key k1 = Key(name);

	Key* k2 = new Key(const_cast<char*>(name));

	String* keyName1 = new String("test");
	Key k3 = Key(keyName1);

	String* keyName2 = new String(name);
	Key* k4 = new Key(keyName2, 10);

	Key k5 = Key(k4);

	CS4500_ASSERT_TRUE(k1.equals(k2));
	CS4500_ASSERT_TRUE(k1.equals(&k3));
	CS4500_ASSERT_TRUE(k1.equals(k4));
	CS4500_ASSERT_TRUE(k1.equals(&k1));
	CS4500_ASSERT_TRUE(k4->equals(&k1));

	delete k2;
	delete k4;

	exit(0);
}

TEST(KeyTests, keyConstructorAndEquality) {ASSERT_EXIT_ZERO(keyConstructor)}


void keyGetter() {
	String* keyName2 = new String("test");
	Key* k4 = new Key(keyName2, 10);

	Key k5 = Key(k4);

	CS4500_ASSERT_TRUE(k4->getKey()->equals(k5.getKey()));
	CS4500_ASSERT_TRUE(k4->getNodeId() == 10);
	CS4500_ASSERT_TRUE(k4->getNodeId() == k5.getNodeId());

	delete k4;

	exit(0);
}

TEST(KeyTests, keyGetter) {ASSERT_EXIT_ZERO(keyGetter)}


void keySetter() {
	String* keyName2 = new String("test");
	Key* k4 = new Key(keyName2, 10);

	Key k5 = Key(k4);

	CS4500_ASSERT_TRUE(k4->getNodeId() == 10);
	CS4500_ASSERT_TRUE(k5.getNodeId() == 10);

	k4->setNodeId(20);

	CS4500_ASSERT_TRUE(k4->getNodeId() == 20);
	CS4500_ASSERT_TRUE(k5.getNodeId() == 10);

	delete k4;

	exit (0);
}

TEST(KeyTests, keySetter) {ASSERT_EXIT_ZERO(keySetter)}

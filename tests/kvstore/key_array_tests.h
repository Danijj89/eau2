#include <string.h>


#include "../../src/kvstore/key_array.h"


void keyArrayConstructor() {
	Key k1 = Key("key");

	KeyArray array = KeyArray();

	for (size_t i = 0; i < 5000; ++i) {
		array.pushBack(&k1);
	}

	exit(0);
}

TEST(KeyArrayTests, keyArrayConstructor) {ASSERT_EXIT_ZERO(keyArrayConstructor)}


void keyArrayGetter() {
	Key k0 = Key("key0");

	Key* k1 = new Key("key1");

	KeyArray array = KeyArray();

	for (size_t i = 0; i < 10; ++i) {
		if (i % 2) {
			array.pushBack(k1);
		} else {
			array.pushBack(&k0);
		}
	}

	for (size_t i = 0; i < 10; ++i) {
		if (i % 2) {
			CS4500_ASSERT_TRUE(array.get(i)->equals(k1));
		} else {
			CS4500_ASSERT_TRUE(array.get(i)->equals(&k0));
		}
	}

	delete k1;

	exit(0);
}

TEST(KeyArrayTests, keyArrayGetter) {ASSERT_EXIT_ZERO(keyArrayGetter)}


void keyArrayGetterOOB() {
	Key k0 = Key("key0");

	Key* k1 = new Key("key1");

	KeyArray array = KeyArray();

	for (size_t i = 0; i < 10; ++i) {
		if (i % 2) {
			array.pushBack(k1);
		} else {
			array.pushBack(&k0);
		}
	}

	array.get(10);

	delete k1;

	exit(0);
}

TEST(KeyArrayTests, keyArrayGetterOOB) {ASSERT_SIGNAL_SIX(keyArrayGetterOOB)}


void keyArraySetter() {
	Key k0 = Key("key0");

	Key* k1 = new Key("key1");

	KeyArray array = KeyArray();

	for (size_t i = 0; i < 10; ++i) {
		if (i % 2) {
			array.pushBack(k1);
		} else {
			array.pushBack(&k0);
		}
	}

	CS4500_ASSERT_TRUE(array.get(5)->equals(k1));

	array.set(5, &k0);

	CS4500_ASSERT_TRUE(array.get(5)->equals(&k0));

	exit(0);
}

TEST(KeyArrayTests, keyArraySetter) {ASSERT_EXIT_ZERO(keyArraySetter)}

#include <gtest/gtest.h>


#include <string.h>


#include "../../src/kvstore/value_array.h"


#define CS4500_ASSERT_TRUE(a)  \
	ASSERT_EQ((a),true);


void valueArrayConstructor() {
	Value v = Value(const_cast<char*>("Blob"), 4);

	ValueArray array = ValueArray();

	for (size_t i = 0; i < 5000; ++i) {
		array.pushBack(&v);
	}

	exit(0);
}

TEST(valueArrayTests, valueConstructor) {ASSERT_EXIT_ZERO(valueArrayConstructor)}


void valueArrayGetter() {
	Value* v0 = new Value(const_cast<char*>("Blob0"), 4);
	Value v1 = Value(const_cast<char*>("Blob1"), 4);

	ValueArray array = ValueArray();

	for (size_t i = 0; i <10; ++i) {
		if (i % 2) {
			array.pushBack(&v1);
		} else {
			array.pushBack(v0);
		}
	}

	for (size_t i = 0; i <10; ++i) {
		if (i % 2) {
			CS4500_ASSERT_TRUE(array.get(i)->equals(&v1));
		} else {
			CS4500_ASSERT_TRUE(array.get(i)->equals(v0));
		}
	}

	delete v0;

	exit(0);
}

TEST(valueArrayTests, valueArrayGetter) {ASSERT_EXIT_ZERO(valueArrayGetter)}


void valueArrayGetterOOB() {
	Value* v0 = new Value(const_cast<char*>("Blob0"), 4);
	Value v1 = Value(const_cast<char*>("Blob1"), 4);

	ValueArray array = ValueArray();

	for (size_t i = 0; i <10; ++i) {
		if (i % 2) {
			array.pushBack(&v1);
		} else {
			array.pushBack(v0);
		}
	}

	for (size_t i = 0; i <11; ++i) {
		if (i % 2) {
			CS4500_ASSERT_TRUE(array.get(i)->equals(&v1));
		} else {
			CS4500_ASSERT_TRUE(array.get(i)->equals(v0));
		}
	}

	delete v0;

	exit(0);
}

TEST(valueArrayTests, valueArrayGetterOOB) {ASSERT_SIGNAL_SIX(valueArrayGetterOOB)}


void valueArraySetter() {
	exit(0);
}

TEST(valueArrayTests, valueArraySetter) {ASSERT_EXIT_ZERO(valueArraySetter)}



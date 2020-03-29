#pragma once


#include <gtest/gtest.h>
#include "../../src/dataframe/val.h"
#include "../../src/util/string.h"

void getType() {
	Val* values[4] {new Bool(), new Int(), new Double(), new Str()};
	ASSERT_EQUALS(values[0]->getType(), 'B');
	ASSERT_EQUALS(values[1]->getType(), 'I');
	ASSERT_EQUALS(values[2]->getType(), 'D');
	ASSERT_EQUALS(values[3]->getType(), 'S');
	delete values[0];
	delete values[1];
	delete values[2];
	delete values[3];
	exit(0);
}

TEST(ValTests, getType) {ASSERT_EXIT_ZERO(getType)}

void setAndGet() {
	Val* values[4] {new Bool(), new Int(), new Double(), new Str()};

	Bool* b = values[0]->asBool();
	b->set(false);
	ASSERT_EQUALS(b->get(), false);

	Int* i = values[1]->asInt();
	i->set(-12);
	ASSERT_EQUALS(i->get(), -12);

	Double* d = values[2]->asDouble();
	d->set(-0.01);
	ASSERT_TRUE(d->get() - -0.01 < 0.000001);

	Str* s = values[3]->asStr();
	String v("test");
	s->set(&v);
	ASSERT_TRUE(s->get()->equals(&v));

	delete values[0];
	delete values[1];
	delete values[2];
	delete values[3];
	exit(0);
}

TEST(ValTests, setAndGet) {ASSERT_EXIT_ZERO(setAndGet)}

void strSetCopyStringVal() {
	Str s = Str();
	String* s1 = new String("s1");
	String s2("s1");
	s.set(s1);
	delete s1;
	ASSERT_TRUE(s.get()->equals(&s2));
	exit(0);
}

TEST(ValTests, strSetCopyStringVal) {ASSERT_EXIT_ZERO(strSetCopyStringVal)}
#pragma once

#include "../../src/dataframe/row.h"

void rowColType() {
	Schema s = Schema();
	s.addColumn('B');
	s.addColumn('I');
	s.addColumn('D');
	s.addColumn('S');
	Row r = Row(&s);
	ASSERT_EQUALS(r.width(), 4);
	ASSERT_EQUALS(r.colType(0), 'B');
	ASSERT_EQUALS(r.colType(1), 'I');
	ASSERT_EQUALS(r.colType(2), 'D');
	ASSERT_EQUALS(r.colType(3), 'S');
	exit(0);
}

TEST(RowTests, rowColType) {ASSERT_EXIT_ZERO(rowColType)}

void rowSetAndGet() {
	Schema s = Schema();
	s.addColumn('B');
	s.addColumn('I');
	s.addColumn('D');
	s.addColumn('S');
	Row r = Row(&s);

	r.set(0, false);
	ASSERT_EQUALS(r.getBool(0), false);

	r.set(1, -12);
	ASSERT_EQUALS(r.getInt(1), -12);

	r.set(2, -0.12);
	ASSERT_TRUE(r.getDouble(2) - -0.12 < 0.000001);

	String s1("s1");
	r.set(3, &s1);
	ASSERT_TRUE(r.getString(3)->equals(&s1));

	exit(0);
}

TEST(RowTests, rowSetAndGet) {ASSERT_EXIT_ZERO(rowSetAndGet)}


void rowFunctionalTest() {
	// Set Up
	Schema* schema = new Schema("BIFS");
	Row* r = new Row(schema);

	CS4500_ASSERT_TRUE(r->width() == 4);

	CS4500_ASSERT_TRUE(schema->colType(0) == 'B');
	CS4500_ASSERT_TRUE(r->colType(0) == 'B');
	bool b0 = true;
	r->set(0, b0);
	bool b1 = r->getBool(0);
	CS4500_ASSERT_TRUE(b0 == b1);
	b0 = !b0;
	r->set(0, b0);
	CS4500_ASSERT_FALSE(b1 == r->getBool(0));

	CS4500_ASSERT_TRUE(schema->colType(1) == 'I');
	CS4500_ASSERT_TRUE(r->colType(1) == 'I');
	int i0 = 2;
	r->set(1, i0);
	int i1 = r->getInt(1);
	CS4500_ASSERT_TRUE(i0 == i1);

	CS4500_ASSERT_TRUE(schema->colType(2) == 'F');
	CS4500_ASSERT_TRUE(r->colType(2) == 'F');
	double d0 = 3.14;
	r->set(2, d0);
	double d1 = r->getDouble(2);
	CS4500_ASSERT_TRUE(d0 == d1);

	CS4500_ASSERT_TRUE(schema->colType(3) == 'S');
	CS4500_ASSERT_TRUE(r->colType(3) == 'S');
	String* s0 = new String("Success");
	r->set(3, s0);
	String* s1 = r->getString(3);
	CS4500_ASSERT_TRUE(s0->equals(s1));

	// Clean Up
	delete s0;
	delete schema;
	delete r;

	// Correct exit
	exit(0);
}

TEST(RowTests, rowFunctionalTest) {
CS4500_ASSERT_EXIT_ZERO(rowFunctionalTest);
}


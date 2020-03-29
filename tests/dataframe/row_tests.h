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
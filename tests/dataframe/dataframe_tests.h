#pragma once


#include <gtest/gtest.h>
#include "../../src/dataframe/dataframe.h"


void dataframeAddColumn() {
    DataFrame df = DataFrame();
    ASSERT_EQUALS(df.ncols_, 0);
	ASSERT_EQUALS(df.getSchema()->width(), 0);
    KVStore kv = KVStore(0);

    String col1("col1");
    BoolColumn* c1 = new BoolColumn(&col1, &kv);
    df.addColumn(c1);
	ASSERT_EQUALS(df.ncols_, 1);
	ASSERT_EQUALS(df.getSchema()->width(), 1);
	ASSERT_EQUALS(df.getSchema()->colType(0), 'B');

	String col2("col2");
    IntColumn* c2 = new IntColumn(&col2, &kv);
	df.addColumn(c2);
	ASSERT_EQUALS(df.ncols_, 2);
	ASSERT_EQUALS(df.getSchema()->width(), 2);
	ASSERT_EQUALS(df.getSchema()->colType(1), 'I');

	String col3("col3");
	DoubleColumn* c3 = new DoubleColumn(&col3, &kv);
	df.addColumn(c3);
	ASSERT_EQUALS(df.ncols_, 3);
	ASSERT_EQUALS(df.getSchema()->width(), 3);
	ASSERT_EQUALS(df.getSchema()->colType(2), 'D');

	String col4("col4");
	StringColumn* c4 = new StringColumn(&col4, &kv);
	df.addColumn(c4);
	ASSERT_EQUALS(df.ncols_, 4);
	ASSERT_EQUALS(df.getSchema()->width(), 4);
	ASSERT_EQUALS(df.getSchema()->colType(3), 'S');

    exit(0);
}

TEST(DataframeTests, dataframeAddColumn) {ASSERT_EXIT_ZERO(dataframeAddColumn)}

void dataframeAddColumnChangeRows() {
	DataFrame df = DataFrame();
	KVStore kv = KVStore(0);
	String col1("col1");
	BoolColumn* c1 = new BoolColumn(&col1, &kv);
	bool vals[100] {false};
	c1->pushBack(vals, 100);
	df.addColumn(c1);
	ASSERT_EQUALS(df.nrows_, 100);
	exit(0);
}

TEST(DataframeTests, dataframeAddColumnChangeRows) {ASSERT_EXIT_ZERO(dataframeAddColumn)}

void dataframeGet() {
	DataFrame df = DataFrame();
	KVStore kv = KVStore(0);

	String col1("col1");
	BoolColumn* c1 = new BoolColumn(&col1, &kv);
	bool v1[2] {true, false};
	c1->pushBack(v1, 2);
	df.addColumn(c1);
	ASSERT_EQUALS(df.getBool(0, 0), true);
	ASSERT_EQUALS(df.getBool(0, 1), false);

	String col2("col2");
	IntColumn* c2 = new IntColumn(&col2, &kv);
	int v2[2] {1, -1};
	c2->pushBack(v2, 2);
	df.addColumn(c2);
	ASSERT_EQUALS(df.getInt(1, 0), 1);
	ASSERT_EQUALS(df.getInt(1, 1), -1);

	String col3("col3");
	DoubleColumn* c3 = new DoubleColumn(&col3, &kv);
	double v3[2] {0.01, -0.01};
	c3->pushBack(v3, 2);
	df.addColumn(c3);
	ASSERT_TRUE(df.getDouble(2, 0) - 0.01 < 0.00001);
	ASSERT_TRUE(df.getDouble(2, 1) - -0.01 < 0.00001);

	String col4("col4");
	StringColumn* c4 = new StringColumn(&col4, &kv);
	StringArray v4 = StringArray();
	String s1("s1");
	String s2("s2");
	v4.pushBack(&s1);
	v4.pushBack(&s2);
	c4->pushBack(&v4);
	df.addColumn(c4);
	ASSERT_TRUE(df.getString(3, 0)->equals(&s1));
	ASSERT_TRUE(df.getString(3, 1)->equals(&s2));

	exit(0);
}

TEST(DataframeTests, dataframeGet) {ASSERT_EXIT_ZERO(dataframeGet)}

void dataframeFillRow() {
	DataFrame df = DataFrame();
	KVStore kv = KVStore(0);

	String col1("col1");
	BoolColumn* c1 = new BoolColumn(&col1, &kv);
	bool v1[2] {true, false};
	c1->pushBack(v1, 2);
	df.addColumn(c1);

	String col2("col2");
	IntColumn* c2 = new IntColumn(&col2, &kv);
	int v2[2] {1, -1};
	c2->pushBack(v2, 2);
	df.addColumn(c2);

	String col3("col3");
	DoubleColumn* c3 = new DoubleColumn(&col3, &kv);
	double v3[2] {0.01, -0.01};
	c3->pushBack(v3, 2);
	df.addColumn(c3);

	String col4("col4");
	StringColumn* c4 = new StringColumn(&col4, &kv);
	StringArray v4 = StringArray();
	String s1("s1");
	String s2("s2");
	v4.pushBack(&s1);
	v4.pushBack(&s2);
	c4->pushBack(&v4);
	df.addColumn(c4);

	Row r = Row(df.getSchema());
	df.fillRow(0, r);
	ASSERT_EQUALS(r.width(), 4);

	ASSERT_EQUALS(r.colType(0), 'B');
	ASSERT_EQUALS(r.getBool(0), true);

	ASSERT_EQUALS(r.colType(1), 'I');
	ASSERT_EQUALS(r.getInt(1), 1);

	ASSERT_EQUALS(r.colType(2), 'D');
	ASSERT_TRUE(r.getDouble(2) - 0.01 < 0.00001);

	ASSERT_EQUALS(r.colType(3), 'S');
	ASSERT_TRUE(r.getString(3)->equals(&s1));

	df.fillRow(1, r);

	ASSERT_EQUALS(r.colType(0), 'B');
	ASSERT_EQUALS(r.getBool(0), false);

	ASSERT_EQUALS(r.colType(1), 'I');
	ASSERT_EQUALS(r.getInt(1), -1);

	ASSERT_EQUALS(r.colType(2), 'D');
	ASSERT_TRUE(r.getDouble(2) - -0.01 < 0.00001);

	ASSERT_EQUALS(r.colType(3), 'S');
	ASSERT_TRUE(r.getString(3)->equals(&s2));

	exit(0);
}

TEST(DataframeTests, dataframeFillRow) {ASSERT_EXIT_ZERO(dataframeFillRow)}

void dataframeGetColsAndGetRows() {
	DataFrame df = DataFrame();
	KVStore kv = KVStore(0);
	String col1("col1");
	BoolColumn* c1 = new BoolColumn(&col1, &kv);
	bool v1[2] {true, false};
	c1->pushBack(v1, 2);
	df.addColumn(c1);
	ASSERT_EQUALS(df.nrows_, 2);
	ASSERT_EQUALS(df.ncols_, 1);
	exit(0);
}

TEST(DataframeTests, dataframeGetColsAndGetRows) {ASSERT_EXIT_ZERO(dataframeGetColsAndGetRows)}

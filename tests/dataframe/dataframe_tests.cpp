/**
 * This is a temporary placeholder.
 */


#include <gtest/gtest.h>

#include "../../src/dataframe/schema.h"
#include "../../src/dataframe/row.h"
#include "../../src/dataframe/dataframe.h"


#define CS4500_ASSERT_TRUE(a)  \
    ASSERT_EQ((a),true);
#define CS4500_ASSERT_FALSE(a) \
    ASSERT_EQ((a),false);
#define CS4500_ASSERT_EXIT_ZERO(a)  \
    ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");
#define CS4500_ASSERT_EXIT_ONE(a)  \
    ASSERT_EXIT(a(), ::testing::ExitedWithCode(1), ".*");
#define CS4500_ASSERT_SIGNAL_SIX(a)  \
    ASSERT_EXIT(a(), ::testing::KilledBySignal(6), ".*");
#define GT_EQUALS(a, b)   ASSERT_EQ(a, b)

void failingConstructorWrongTypes() {
    Schema *s = new Schema("asdf");
}

TEST(SchemaTests, failingConstructorWrongTypes
) {
CS4500_ASSERT_EXIT_ONE(failingConstructorWrongTypes);
}

void failingConstructorNullptr() {
    Schema *s = new Schema("asdf");
}

TEST(SchemaTests, failingConstructorNullptr
) {
CS4500_ASSERT_EXIT_ONE(failingConstructorNullptr);
}

void schemaCopyConstructor() {

	Schema* scm = new Schema("BIFS");
	scm->add_column('I');
	Schema* scm_copy = new Schema(*scm);

	CS4500_ASSERT_TRUE(scm->col_type(0) == scm_copy->col_type(0));
	CS4500_ASSERT_TRUE(scm->col_type(1) == scm_copy->col_type(1));
	CS4500_ASSERT_TRUE(scm->col_type(2) == scm_copy->col_type(2));
	CS4500_ASSERT_TRUE(scm->col_type(3) == scm_copy->col_type(3));
	CS4500_ASSERT_TRUE(scm->col_type(4) == scm_copy->col_type(4));
	CS4500_ASSERT_TRUE(scm->width() == scm_copy->width());

	delete scm;
	delete scm_copy;
	exit(0);
}

TEST(SchemaTests, schemaCopyConstructor
) {
CS4500_ASSERT_EXIT_ZERO(schemaCopyConstructor);
}

void colTypeIndexOutOfBound() {
    Schema *s = new Schema("I");
    s->col_type(1);
}

TEST(SchemaTests, colTypeIndexOutOfBound
) {
CS4500_ASSERT_EXIT_ONE(colTypeIndexOutOfBound);
}

void schemaFunctionalTest() {
    Schema *s = new Schema("ISFB");

    CS4500_ASSERT_TRUE(s->col_type(0) == 'I');
    CS4500_ASSERT_TRUE(s->col_type(3) == 'B');
    CS4500_ASSERT_TRUE(s->width() == 4);

    s->add_column('I');
    CS4500_ASSERT_TRUE(s->col_type(4) == 'I');
    CS4500_ASSERT_TRUE(s->width() == 5);

    delete s;
    exit(0);
}

TEST(SchemaTests, schemaFunctionalTest
) {
CS4500_ASSERT_EXIT_ZERO(schemaFunctionalTest);
}

void varArgConstructor() {
    IntColumn* col = new IntColumn(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    String* s1 = new String("s1");
    String* s2 = new String("s2");
    StringColumn* col2 = new StringColumn(2, s1, s2);
    delete s1;
    delete s2;
    delete col;
    delete col2;
    exit(0);
}

TEST(ColumnTests, varArgConstructor) {
    CS4500_ASSERT_EXIT_ZERO(varArgConstructor)
}

void pushBackAndGrow() {
    size_t rlen = 100;
    IntColumn* col = new IntColumn();
    CS4500_ASSERT_TRUE(col->size() == 0);
    for (size_t i = 0; i < rlen; ++i) {
		col->pushBack(i);
    }
    CS4500_ASSERT_TRUE(col->size() == 100);
	col->pushBack(1000);
    CS4500_ASSERT_TRUE(col->size() == 101);
    CS4500_ASSERT_TRUE(col->get(100) == 1000);
    delete col;

    StringColumn* col2 = new StringColumn();
    CS4500_ASSERT_TRUE(col2->size() == 0);
    String* s0 = new String("hi");
    for (size_t i = 0; i < rlen; ++i) {
		col2->pushBack(s0);
    }
    CS4500_ASSERT_TRUE(col2->size() == 100);
    String* s1 = new String("hello");
	col2->pushBack(s1);
    CS4500_ASSERT_TRUE(col2->size() == 101);
    String* s2 = new String("hi");
    CS4500_ASSERT_TRUE(col2->get(0)->equals(s2));
    CS4500_ASSERT_TRUE(col2->get(100)->equals(s1));
    delete col2;
    delete s2;
    delete s0;
    exit(0);
}

TEST(ColumnTests, pushBackAndGrow) {
CS4500_ASSERT_EXIT_ZERO(pushBackAndGrow)
}

void stringColumnSetOwnership() {
    StringColumn* col = new StringColumn();
    String* s1 = new String("s1");
	col->pushBack(s1);
    // we clone the s1 since the pointer value of the stored string is != s1
    CS4500_ASSERT_FALSE(col->get(0) == (s1));
    String* s2 = new String("s2");
    col->set(0, s2);
    CS4500_ASSERT_TRUE(col->get(0)->equals(s2));
    delete s1;
    delete s2;
    delete col;
    exit(0);
}

TEST(ColumnTests, stringColumnSetOwnership) {
CS4500_ASSERT_EXIT_ZERO(stringColumnSetOwnership)
}

void stringColumnFunctionalTest() {
    String* s1 = new String("s1");
    String* s2 = new String("s2");
    String* s3 = new String("s3");
    StringColumn* col = new StringColumn(2, s1, s2);
    CS4500_ASSERT_TRUE(col->size() == 2);
    CS4500_ASSERT_TRUE(col->get(0)->equals(s1));
    CS4500_ASSERT_TRUE(col->get(1)->equals(s2));
    col->set(0, s3);
    CS4500_ASSERT_TRUE(col->size() == 2);
    CS4500_ASSERT_TRUE(col->get(0)->equals(s3));
	col->pushBack(s1);
    CS4500_ASSERT_TRUE(col->size() == 3);
    CS4500_ASSERT_TRUE(col->get(2)->equals(s1));
    CS4500_ASSERT_TRUE(col->indexOf("s3") == 0);
    CS4500_ASSERT_TRUE(col->indexOf("s4") == -1);

    delete s1;
    delete s2;
    delete col;
    exit(0);
}

TEST(ColumnTests, stringColumnFunctionalTest) {
CS4500_ASSERT_EXIT_ZERO(stringColumnFunctionalTest)
}

void rowFunctionalTest() {
	// Set Up
	Schema* schema = new Schema("BIFS");
	Row* r = new Row(*schema);

	CS4500_ASSERT_TRUE(r->width() == 4);

	CS4500_ASSERT_TRUE(schema->col_type(0) == 'B');
	CS4500_ASSERT_TRUE(r->col_type(0) == 'B');
	bool b0 = true;
	r->set(0, b0);
	bool b1 = r->get_bool(0);
	CS4500_ASSERT_TRUE(b0 == b1);
	b0 = !b0;
	r->set(0, b0);
	CS4500_ASSERT_FALSE(b1 == r->get_bool(0));

	CS4500_ASSERT_TRUE(schema->col_type(1) == 'I');
	CS4500_ASSERT_TRUE(r->col_type(1) == 'I');
	int i0 = 2;
	r->set(1, i0);
	int i1 = r->get_int(1);
	CS4500_ASSERT_TRUE(i0 == i1);

	CS4500_ASSERT_TRUE(schema->col_type(2) == 'F');
	CS4500_ASSERT_TRUE(r->col_type(2) == 'F');
	float f0 = 3.14;
	r->set(2, f0);
	float f1 = r->get_float(2);
	CS4500_ASSERT_TRUE(f0 == f1);

	CS4500_ASSERT_TRUE(schema->col_type(3) == 'S');
	CS4500_ASSERT_TRUE(r->col_type(3) == 'S');
	String* s0 = new String("Success");
	r->set(3, s0);
	String* s1 = r->get_string(3);
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

void constructors() {
    Schema* s = new Schema("ISFB");
    DataFrame* df = new DataFrame(*s);
    // Test that the schema is copied
    CS4500_ASSERT_TRUE((df->get_schema()) != s);
    CS4500_ASSERT_TRUE(df->nrows() == 0);
    CS4500_ASSERT_TRUE(df->ncols() == 4);

    IntColumn* col1 = new IntColumn();
    df->add_column(col1);
    CS4500_ASSERT_TRUE(df->ncols() == 5);
    CS4500_ASSERT_TRUE(df->get_schema()->col_type(4) == 'I');

    s->add_column('I');
    Row* row1 = new Row(*s);
    df->add_row(*row1);
    CS4500_ASSERT_TRUE(df->nrows() == 1);

    DataFrame* df2 = new DataFrame(*df);
    CS4500_ASSERT_TRUE(df2->nrows() == 0);
    CS4500_ASSERT_TRUE(df2->ncols() == 5);
    CS4500_ASSERT_TRUE((df2->get_schema()) != (df->get_schema()));

    delete s;
    delete df;
    delete col1;
    delete row1;
    delete df2;
    exit(0);
}

TEST(DataFrameTest, constructors) {
CS4500_ASSERT_EXIT_ZERO(constructors);
}

void addRowFillRow() {
    Schema* s = new Schema();
    DataFrame* df = new DataFrame(*s);
    IntColumn* col1 = new IntColumn(2, -1, 2);
    BoolColumn* col2 = new BoolColumn(2, false, true);
    FloatColumn* col3 = new FloatColumn(2, 0.1, 0.2);
    String* s1 = new String("s1");
    String* s2 = new String("s2");
    StringColumn* col4 = new StringColumn(2, s1, s2);
    df->add_column(col1);
    df->add_column(col2);
    df->add_column(col3);
    df->add_column(col4);

    // can delete since we clone
    delete col1;
    delete col2;
    delete col3;
    delete col4;

    CS4500_ASSERT_TRUE(df->nrows() == 2);
    CS4500_ASSERT_TRUE(df->ncols() == 4);

    Row* row1 = new Row(*df->get_schema());
    df->fill_row(0, *row1);
    CS4500_ASSERT_TRUE(row1->get_int(0) == -1);
    CS4500_ASSERT_TRUE(row1->get_bool(1) == false);
    CS4500_ASSERT_TRUE(row1->get_float(2) - 0.1 < 0.00001);
    CS4500_ASSERT_TRUE(row1->get_string(3)->equals(s1));

    df->add_row(*row1);
    delete row1;
    CS4500_ASSERT_TRUE(df->nrows() == 3);
    CS4500_ASSERT_TRUE(df->get_int(0, 2) == -1);
    CS4500_ASSERT_TRUE(df->get_bool(1, 2) == false);
    CS4500_ASSERT_TRUE(df->get_float(2, 2) - 0.1 < 0.00001);
    CS4500_ASSERT_TRUE(df->get_string(3, 2)->equals(s1));

    delete s1;
    delete s2;
    delete s;
    delete df;

    exit(0);
}

TEST(DataFrameTest, addRowFillRow) {
CS4500_ASSERT_EXIT_ZERO(addRowFillRow);
}

void addColumn() {
    Schema* s = new Schema();
    DataFrame* df = new DataFrame(*s);
    IntColumn* col1 = new IntColumn(2, 1, 2);
    BoolColumn* col2 = new BoolColumn(2, false, true);
    FloatColumn* col3 = new FloatColumn(2, 0.1, 0.2);
    String* s1 = new String("s1");
    String* s2 = new String("s2");
    StringColumn* col4 = new StringColumn(2, s1, s2);
    CS4500_ASSERT_TRUE(df->ncols() == 0);
    df->add_column(col1);
    CS4500_ASSERT_TRUE(df->ncols() == 1);
    df->add_column(col2);
    df->add_column(col3);
    df->add_column(col4);
    CS4500_ASSERT_TRUE(df->ncols() == 4);

    delete col1;
    delete col2;
    delete col3;
    delete col4;
    delete s1;
    delete s2;
    delete s;
    delete df;
    exit(0);
}

TEST(DataFrameTest, addColumn) {
CS4500_ASSERT_EXIT_ZERO(addColumn);
}

void basic_example() {
    Schema s("II");

    DataFrame df(s);
    Row  r(*df.get_schema());
    for(size_t i = 0; i <  1000 * 1000; i++) {
        r.set(0,(int)i);
        r.set(1,(int)i+1);
        df.add_row(r);
    }
    GT_EQUALS(df.get_int((size_t)0,1), 1);
    exit(0);
}

TEST(a4, basic_example){ CS4500_ASSERT_EXIT_ZERO(basic_example); }

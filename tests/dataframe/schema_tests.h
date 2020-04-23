#include "../../src/dataframe/schema.h"


void schemaIsValidTypes() {
	Schema s = Schema();
	ASSERT_TRUE(s.isValidType_('B'));
	ASSERT_TRUE(s.isValidType_('I'));
	ASSERT_TRUE(s.isValidType_('D'));
	ASSERT_TRUE(s.isValidType_('S'));
	ASSERT_FALSE(s.isValidType_('F'));
	exit(0);
}

TEST(SchemaTests, schemaIsValidTypes) {ASSERT_EXIT_ZERO(schemaIsValidTypes)}

void schemaAddColumn() {
	Schema s = Schema();
	ASSERT_TRUE(s.width() == 0);
	s.addColumn('B');
	ASSERT_TRUE(s.width() == 1);
	ASSERT_TRUE(s.colType(0) == 'B');
	s.addColumn('I');
	ASSERT_TRUE(s.width() == 2);
	ASSERT_TRUE(s.colType(0) == 'B');
	ASSERT_TRUE(s.colType(1) == 'I');
	exit(0);
}

TEST(SchemaTests, schemaAddColumn) {ASSERT_EXIT_ZERO(schemaAddColumn)}

void schemaAddColumnInvalidType() {
	Schema s = Schema();
	s.addColumn('F');
}

TEST(SchemaTests, schemaAddColumnInvalidType) {ASSERT_SIGNAL_SIX(schemaAddColumnInvalidType)}


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
	scm->addColumn('I');
	Schema* scm_copy = new Schema(*scm);

	CS4500_ASSERT_TRUE(scm->colType(0) == scm_copy->colType(0));
	CS4500_ASSERT_TRUE(scm->colType(1) == scm_copy->colType(1));
	CS4500_ASSERT_TRUE(scm->colType(2) == scm_copy->colType(2));
	CS4500_ASSERT_TRUE(scm->colType(3) == scm_copy->colType(3));
	CS4500_ASSERT_TRUE(scm->colType(4) == scm_copy->colType(4));
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
    s->colType(1);
}

TEST(SchemaTests, colTypeIndexOutOfBound
) {
CS4500_ASSERT_EXIT_ONE(colTypeIndexOutOfBound);
}

void schemaFunctionalTest() {
    Schema *s = new Schema("ISFB");

    CS4500_ASSERT_TRUE(s->colType(0) == 'I');
    CS4500_ASSERT_TRUE(s->colType(3) == 'B');
    CS4500_ASSERT_TRUE(s->width() == 4);

    s->addColumn('I');
    CS4500_ASSERT_TRUE(s->colType(4) == 'I');
    CS4500_ASSERT_TRUE(s->width() == 5);

    delete s;
    exit(0);
}

TEST(SchemaTests, schemaFunctionalTest
) {
CS4500_ASSERT_EXIT_ZERO(schemaFunctionalTest);
}

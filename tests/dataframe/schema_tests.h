#pragma once
#include <gtest/gtest.h>
#include "../../src/dataframe/schema.h"

    
Schema* s = nullptr;
    
void setup() {
	s = new Schema();
}

void tearDown() {
	delete s;
}

void isValidTypes() {
	setup();
	ASSERT_TRUE(s->isValidType_('B'));
	ASSERT_TRUE(s->isValidType_('I'));
	ASSERT_TRUE(s->isValidType_('D'));
	ASSERT_TRUE(s->isValidType_('S'));
	ASSERT_FALSE(s->isValidType_('F'));
	tearDown();
	exit(0);
}

TEST(SchemaTests, isValidTypes) {ASSERT_EXIT_ZERO(isValidTypes)}

void addColumn() {
	setup();
	ASSERT_TRUE(s->width() == 0);
	s->addColumn('B');
	ASSERT_TRUE(s->width() == 1);
	ASSERT_TRUE(s->colType(0) == 'B');
	s->addColumn('I');
	ASSERT_TRUE(s->width() == 2);
	ASSERT_TRUE(s->colType(0) == 'B');
	ASSERT_TRUE(s->colType(1) == 'I');
	tearDown();
	exit(0);
}

TEST(SchemaTests, addColumn) {ASSERT_EXIT_ZERO(addColumn)}

void addColumnInvalidType() {
	setup();
	s->addColumn('F');
}

TEST(SchemaTests, addColumnInvalidType) {ASSERT_SIGNAL_SIX(addColumnInvalidType)}
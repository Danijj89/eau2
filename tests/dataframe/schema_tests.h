#pragma once
#include <gtest/gtest.h>
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
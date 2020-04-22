#include "../../src/util/helper.h"


void parseUIntTest() {
	char* uint1 = const_cast<char*>("12345");
	size_t u1 = parse_uint(uint1);
	ASSERT_EQUALS(u1, 12345);

	char* uint2 = const_cast<char*>("23456454");
	size_t u2 = parse_uint(uint2);
	ASSERT_EQUALS(u2, 23456454);

	exit(0);
}

TEST(HelperTests, parseUIntTest) {ASSERT_EXIT_ZERO(parseUIntTest)}


void parseNegativeTest() {
	char* uint1 = const_cast<char*>("-12345");
	size_t u1 = parse_uint(uint1);
	ASSERT_EQUALS(u1, -12345); // True, but do not use this
	exit(0);
}

TEST(HelperTests, parseNegativeTest) {ASSERT_EXIT_ZERO(parseNegativeTest)}


void parseDecimalTest() {
	char* uint1 = const_cast<char*>("12345.4321");
	size_t u1 = parse_uint(uint1);
	exit(0);
}

TEST(HelperTests, parseDecimalTest) {ASSERT_SIGNAL_SIX(parseDecimalTest)}
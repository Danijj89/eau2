#include "../../../src/application/wordcount/adder.h"


void adderNotTestedNotice() {
	// This class should not be tested independently.

	exit(0);
}

TEST(AdderTests, adderNotTestedNotice) {ASSERT_EXIT_ZERO(adderNotTestedNotice)}
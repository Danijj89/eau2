#include "../../src/dataframe/rower.h"


void rowerNotTestedNotice() {
	// This class cannot be tested independently.

	exit(0);
}

TEST(RowerTests, rowerNotTestedNotice) {ASSERT_EXIT_ZERO(rowerNotTestedNotice)}
#include "../../src/util/string.h"


void stringNotTestedNotice() {
	// string and strbuff is for common use in 4500
	// This is used like a standard library, and thus not tested
	exit(0);
}

TEST(StringAndStrBuffTests, stringNotTestedNotice) {ASSERT_EXIT_ZERO(stringNotTestedNotice)}
#include "../../src/util/object.h"


void objectNotTestedNotice() {
	// object is for common use in 4500
	// This is used like a standard library, and thus not tested
	exit(0);
}

TEST(ObjectTests, objectNotTestedNotice) {ASSERT_EXIT_ZERO(objectNotTestedNotice)}
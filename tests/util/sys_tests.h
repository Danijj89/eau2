#include "../../src/util/sys.h"


void sysNotTestedNotice() {
	// sys is for common use in 4500
	// This is used like a standard library, and thus not tested
	exit(0);
}

TEST(SysTests, sysNotTestedNotice) {ASSERT_EXIT_ZERO(sysNotTestedNotice)}
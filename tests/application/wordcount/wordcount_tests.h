#include "../../../src/application/wordcount/wordcount.h"


void workcountNotTestedNotice() {
	// This class should not be tested independently. It has been tested via a
	// funcational test, which is the main demo.

	exit(0);
}

TEST(WordcountTests, workcountNotTestedNotice) {ASSERT_EXIT_ZERO(workcountNotTestedNotice)}
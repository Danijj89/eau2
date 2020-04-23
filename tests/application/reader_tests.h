#include "../../src/application/reader.h"


void readerNotTestedNotice() {
	// This class cannot be tested independently.

	exit(0);
}

TEST(ReaderTests, readerNotTestedNotice) {ASSERT_EXIT_ZERO(readerNotTestedNotice)}
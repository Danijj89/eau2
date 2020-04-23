#include "../../src/application/writer.h"


void writerNotTestedNotice() {
	// This class cannot be test independently.

	exit(0);
}

TEST(WriterTests, writerNotTestedNotice) {ASSERT_EXIT_ZERO(writerNotTestedNotice)}
#include "../../src/network/node_info.h"


void pollfdsNotTestedNotice() {
	exit(0);
}

TEST(PollfdsTests, pollfdsNotTestedNotice) {ASSERT_EXIT_ZERO(pollfdsNotTestedNotice)}
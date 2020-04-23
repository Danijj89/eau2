#include "../../src/network/node_info.h"


void nodeNotTestedNotice() {
	exit(0);
}

TEST(NodeTests, nodeNotTestedNotice) {ASSERT_EXIT_ZERO(nodeNotTestedNotice)}
#include "../../src/network/node_info.h"


void nodeNotTestedNotice() {
	// This class contains primitive node functionalities that cannot be easily
	// tested. Instead the test will be performed as part of a functional test
	// for node.

	exit(0);
}

TEST(NodeTests, nodeNotTestedNotice) {ASSERT_EXIT_ZERO(nodeNotTestedNotice)}
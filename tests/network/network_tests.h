#include "../../src/network/network.h"


void networkNotTestedNotice() {
	// This class contains primitive network functions that cannot be easily
	// tested. Instead the test will be performed as part of a functional test
	// for node.
	exit(0);
}

TEST(NetworkTests, networkNotTestedNotice) {ASSERT_EXIT_ZERO(networkNotTestedNotice)}
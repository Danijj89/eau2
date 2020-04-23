#include "../../src/kvstore/KVStore.h"


void kvstoreNotTestedNotice() {
	// This class contains primitive kvstore functionalities that cannot be
	// easily tested. Instead the test will be performed as part of a
	// functional test for node.
	exit(0);
}

TEST(KVStoreTests, kvstoreNotTestedNotice) {ASSERT_EXIT_ZERO(kvstoreNotTestedNotice)}

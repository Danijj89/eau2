#include "../../src/network/node_info_array.h"


void nodeInfoArrayNotCompletelyTestedNotice() {
	exit(0);
}

TEST(NodeInfoArrayTests, nodeInfoArrayNotCompletelyTestedNotice) {ASSERT_EXIT_ZERO(nodeInfoArrayNotCompletelyTestedNotice)}


void nodeInfoArrayConstructorDestructorTest() {
	NodeInfoArray nia1 = NodeInfoArray();
	NodeInfoArray* nia2 = new NodeInfoArray();

	delete nia2;

	exit(0);
}

TEST(NodeInfoArrayTests, nodeInfoArrayConstructorDestructorTest) {ASSERT_EXIT_ZERO(nodeInfoArrayConstructorDestructorTest)}
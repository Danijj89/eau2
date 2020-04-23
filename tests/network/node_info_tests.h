#include "../../src/network/node_info.h"


void nodeInfoNotCompletelyTestedNotice() {
	exit(0);
}

TEST(NodeInfoTests, nodeInfoNotCompletelyTestedNotice) {ASSERT_EXIT_ZERO(nodeInfoNotCompletelyTestedNotice)}


void nodeInfoConstructorDestructorTest() {
	String s1 = String("info1");
	String* s2 = new String("info2");

	NodeInfo info1 = NodeInfo(&s1, 12345);
	NodeInfo* info2 = new NodeInfo(s2, 54321);

	delete info2;

	exit(0);
}

TEST(NodeInfoTests, nodeInfoConstructorDestructorTest) {ASSERT_EXIT_ZERO(nodeInfoConstructorDestructorTest)}
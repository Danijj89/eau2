#include <gtest/gtest.h>

// #include "../sorer/sorer.h"


#define CS4500_ASSERT_TRUE(a)  \
	ASSERT_EQ((a),true);
#define CS4500_ASSERT_FALSE(a) \
	ASSERT_EQ((a),false);
#define CS4500_ASSERT_EXIT_ZERO(a)  \
	ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");

void sample_test() {
	exit(0);
}

TEST(SAMPLE, sample_test) {
	CS4500_ASSERT_EXIT_ZERO(sample_test)
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

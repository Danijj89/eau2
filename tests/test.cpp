#include <gtest/gtest.h>

//#define ASSERT_TRUE(a)  \
//    ASSERT_EQ((a),true);
//#define ASSERT_FALSE(a) \
//    ASSERT_EQ((a),false);
#define ASSERT_EXIT_ZERO(a)  \
    ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");
#define ASSERT_EXIT_ONE(a)  \
    ASSERT_EXIT(a(), ::testing::ExitedWithCode(1), ".*");
#define ASSERT_SIGNAL_SIX(a)  \
    ASSERT_EXIT(a(), ::testing::KilledBySignal(6), ".*");
#define ASSERT_EQUALS(a, b)   ASSERT_EQ(a, b)


#include "dataframe/schema_tests.h"
#include "dataframe/row_tests.h"
#include "kvstore/key_tests.h"
#include "kvstore/value_tests.h"
#include "kvstore/key_array_tests.h"
#include "kvstore/kvstore_tests.h"


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

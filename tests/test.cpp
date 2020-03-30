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
#include "dataframe/val_tests.h"
#include "dataframe/row_tests.h"

#include "kvstore/key_tests.h"
#include "kvstore/value_tests.h"
#include "kvstore/key_array_tests.h"
#include "kvstore/value_array_tests.h"
#include "kvstore/kvstore_tests.h"

#include "serdes/serializer_tests.h"
#include "serdes/deserializer_tests.h"


//void sorerDFIntegrationTest() {
//	Sorer s;
//	s.initialize_sorer("../data/test.sor", 0, 1000);
//	DataFrame* df = s.getAsDF();
//	Schema* schema = df->get_schema();
//	CS4500_ASSERT_TRUE(schema->col_type(0) == 'B');
//	CS4500_ASSERT_TRUE(schema->col_type(1) == 'I');
//	CS4500_ASSERT_TRUE(schema->col_type(2) == 'F');
//	CS4500_ASSERT_TRUE(schema->col_type(3) == 'S');
//	CS4500_ASSERT_TRUE(df->get_bool(0, 0));
//	CS4500_ASSERT_TRUE(df->get_int(1, 1) == 2);
//	CS4500_ASSERT_TRUE(df->get_float(2, 0) - 1.2 < 0.000001);
//	String* str = new String("bye");
//	CS4500_ASSERT_TRUE(df->get_string(3, 0)->equals(str));
//	delete df;
//	delete str;
//	exit(0);
//}
//
//TEST(SORER_TO_DF, sorerDFIntegrationTest) {
//	CS4500_ASSERT_EXIT_ZERO(sorerDFIntegrationTest)
//}
//
//
//void keyUnitTests() {

//
//	Key k3 = Key(k1.getKey(), 0);
//	CS4500_ASSERT_TRUE(k3.getNodeId() == 0);
//
//	k3.setNodeId(3);
//	CS4500_ASSERT_TRUE(k3.getNodeId() == 3);
//
//	String* str = k3.getKey();
//	CS4500_ASSERT_TRUE(str->equals(k1.getKey()));
//
//	delete k2;
//	exit(0);
//}
//
//TEST(KEY, keyUnitTest) {
//	CS4500_ASSERT_EXIT_ZERO(keyUnitTests)
//}
//
//
//void keyArrayUnitTests() {
//	Key k1 = Key("test1");
//	Key k2 = Key("test2");
//	KeyArray ka = KeyArray();
//
//	ka.pushBack(&k1);
//	ka.pushBack(&k2);
//
//	exit(0);
//}
//
//TEST(KEYARRAY, keyArrayUnitTests) {
//	CS4500_ASSERT_EXIT_ZERO(keyArrayUnitTests)
//}


int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

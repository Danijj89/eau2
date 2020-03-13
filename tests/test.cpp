#include <gtest/gtest.h>


#include "../src/sorer/sorer.h"
#include "../src/dataframe/dataframe.h"
#include "dataframe/dataframe_tests.cpp"


#define CS4500_ASSERT_TRUE(a)  \
	ASSERT_EQ((a),true);
#define CS4500_ASSERT_FALSE(a) \
	ASSERT_EQ((a),false);
#define CS4500_ASSERT_EXIT_ZERO(a)  \
	ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");


void sorerDFIntegrationTest() {
	Sorer s;
	s.initialize_sorer("../data/test.sor", 0, 1000);
	DataFrame* df = s.getAsDF();
	Schema* schema = df->get_schema();
	CS4500_ASSERT_TRUE(schema->col_type(0) == 'B');
	CS4500_ASSERT_TRUE(schema->col_type(1) == 'I');
	CS4500_ASSERT_TRUE(schema->col_type(2) == 'F');
	CS4500_ASSERT_TRUE(schema->col_type(3) == 'S');
	CS4500_ASSERT_TRUE(df->get_bool(0, 0));
	CS4500_ASSERT_TRUE(df->get_int(1, 1) == 2);
	CS4500_ASSERT_TRUE(df->get_float(2, 0) - 1.2 < 0.000001);
	String* str = new String("bye");
	CS4500_ASSERT_TRUE(df->get_string(3, 0)->equals(str));
	delete df;
	delete str;
	exit(0);
}

TEST(SORER_TO_DF, sorerDFIntegrationTest) {
	CS4500_ASSERT_EXIT_ZERO(sorerDFIntegrationTest)
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

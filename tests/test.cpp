#include <gtest/gtest.h>


#include "../sorer/sorer.h"
#include "../dataframe/dataframe.h"


#define CS4500_ASSERT_TRUE(a)  \
	ASSERT_EQ((a),true);
#define CS4500_ASSERT_FALSE(a) \
	ASSERT_EQ((a),false);
#define CS4500_ASSERT_EXIT_ZERO(a)  \
	ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*");


void sampleTest() {
	exit(0);
}

TEST(SAMPLE, sampleTest) {
	CS4500_ASSERT_EXIT_ZERO(sampleTest)
}


void sorerTest() {
	Sorer s;
	s.initialize_sorer("../data/test.sor", 0, 1000);
	DataFrame* df = s.getAsDF();

	CS4500_ASSERT_TRUE(df->get_bool(0, 0));

	CS4500_ASSERT_TRUE(df->get_int(1, 1) == 2);
	
	float f = df->get_float(0, 2);
	CS4500_ASSERT_TRUE(f < (1.2 + 0.001));
	CS4500_ASSERT_TRUE(f > (1.2 - 0.001));

	String* str = new String("bye");
	CS4500_ASSERT_TRUE(df->get_string(0, 3)->equals(str));

	delete df;
	delete str;
	exit(0);
}

TEST(SORER, sorerTest) {
	CS4500_ASSERT_EXIT_ZERO(sorerTest)
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

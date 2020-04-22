#include "../../src/util/array.h"
#include "../../src/util/string.h"


void arrayConstructorDestructorTest() {
	Array a = Array();
	Array* b = new Array();
	Array c = Array(100);
	Array* d = new Array(100);

	delete b;
	delete d;

	exit(0);
}

TEST(ArrayTests, arrayConstructorDestructorTest) {ASSERT_EXIT_ZERO(arrayConstructorDestructorTest)}


void arrayPushBackTest() {
	Array a = Array();
	String* s = new String("Test");

	a.pushBack(s);
	a.pushBack(s);
	a.pushBack(s);
	a.pushBack(s);
	a.pushBack(s);
	a.pushBack(s);

	exit(0);
}

TEST(ArrayTests, arrayPushBackTest) {ASSERT_EXIT_ZERO(arrayPushBackTest)}


void arraySetGetTest() {
	Array a = Array();
	String* s1 = new String("Test1");
	String* s2 = new String("Test2");

	a.pushBack(s1);
	a.pushBack(s2);

	ASSERT_TRUE(a.get(0)->equals(s1));
	ASSERT_TRUE(a.get(1) == s2);

	a.set(0, s2);

	ASSERT_TRUE(a.get(0) == a.get(1));

	exit(0);
}

TEST(ArrayTests, arraySetGetTest) {ASSERT_EXIT_ZERO(arraySetGetTest)}


void arrayIndexOfTest() {
	Array a = Array();
	String* s1 = new String("Test");
	String* s2 = new String("Test");

	a.pushBack(s1);
	a.pushBack(s2);
	a.pushBack(s2);
	a.pushBack(s1);

	ASSERT_TRUE(a.indexOf(s1) == 0);
	ASSERT_TRUE(a.indexOf(s2) == 0); // String uses extensional equality

	exit(0);
}

TEST(ArrayTests, arrayIndexOfTest) {ASSERT_EXIT_ZERO(arrayIndexOfTest)}

void arrayFindTest() {
	Array a = Array();
	String* s1 = new String("Test1");
	String* s2 = new String("Test2");
	String* s3 = new String("Test3");

	a.pushBack(s1);
	a.pushBack(s2);

	String* result = (String*) a.find(s2);
	ASSERT_TRUE(result->equals(s2));
	ASSERT_TRUE(result == s2);

	ASSERT_EQUALS(a.find(s3), nullptr);

	delete s3;

	exit(0);
}
TEST(ArrayTests, arrayFindTest) {ASSERT_EXIT_ZERO(arrayFindTest)}
#include "../../src/serialization/deserializer.h"


void deserializerConstructorTest() {
	Deserializer des = Deserializer();

	exit(0);
}

TEST(DeserializerTests, deserializerConstructorTest) {ASSERT_EXIT_ZERO(deserializerConstructorTest)}


void deserializerDeserializationTest() {
		Serializer ser = Serializer();

		bool demoBool = true;
		ser.serializeBool(demoBool);

		int demoInt = 12345;
		ser.serializeInt(demoInt);

		size_t demoSizeT = 123456789;
		ser.serializeSizeT(demoSizeT);

		double demoDouble = 3.14159265358979323846264338327950288419716939937510;
		ser.serializeDouble(demoDouble);

		String* demoString = new String("I'm part of the demo.");
		ser.serializeString(demoString);

		String* serialized = ser.get();
		size_t counter = 0;
		char* rawBuff = serialized->c_str();

		Deserializer des = Deserializer();

		bool resultBool = des.deserializeBool(rawBuff, &counter);
		ASSERT_EQUALS(demoBool, resultBool);

		int resultInt = des.deserializeInt(rawBuff, &counter);
		ASSERT_EQUALS(demoInt, resultInt);

		size_t resultSizeT = des.deserializeSizeT(rawBuff, &counter);
		ASSERT_EQUALS(demoSizeT, resultSizeT);

		double resultDouble = des.deserializeDouble(rawBuff, &counter);
		ASSERT_EQUALS(demoDouble, resultDouble);

		String* resultString = des.deserializeString(rawBuff, &counter);
		ASSERT_TRUE(demoString->equals(resultString));


		delete demoString;
		delete serialized;

		exit(0);
}

TEST(DeserializerTests, deserializerDeserializationTest) {ASSERT_EXIT_ZERO(deserializerDeserializationTest)}

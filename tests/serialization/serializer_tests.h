#include "../../src/serialization/deserializer.h"


#include <iostream>


void serializerConstructorTest() {
	Serializer ser1 = Serializer();
	Serializer* ser2 = new Serializer();

	delete ser2;

	exit(0);
}

TEST(SerializerTests, serializerConstructorTest) {ASSERT_EXIT_ZERO(serializerConstructorTest)}


void serializerSerializationTest() {
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
	delete demoString;

	delete ser.get();

	exit(0);
}

TEST(SerializerTests, serializerSerializationTest) {ASSERT_EXIT_ZERO(serializerSerializationTest)}

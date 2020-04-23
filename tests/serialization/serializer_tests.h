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
	Serializer ser1 = Serializer();
	bool demoBool = true;
	ser1.serializeBool(demoBool);

	Serializer ser2 = Serializer();
	int demoInt = 12345;
	ser2.serializeInt(demoInt);

	Serializer ser3 = Serializer();
	size_t demoSizeT = 123456789;
	ser3.serializeSizeT(demoSizeT);

	Serializer ser4 = Serializer();
	double demoDouble = 3.14159265358979323846264338327950288419716939937510;
	ser4.serializeDouble(demoDouble);

	Serializer ser5 = Serializer();
	String* demoString = new String("I'm part of the demo.");
	ser5.serializeString(demoString);
	delete demoString;

	delete ser1.get();
	delete ser2.get();
	delete ser3.get();
	delete ser4.get();
	delete ser5.get();

	exit(0);
}

TEST(SerializerTests, serializerSerializationTest) {ASSERT_EXIT_ZERO(serializerSerializationTest)}

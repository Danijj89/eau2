#include <gtest/gtest.h>

#include <iostream>


#include "../../src/serdes/serializer.h"


void serializerConstructorAndGetter() {
	Serializer ser1 = Serializer();
	Serializer* ser2 = new Serializer();

	CS4500_ASSERT_TRUE(ser1.get_size() == ser2->get_size());
	CS4500_ASSERT_TRUE(ser1.get_size() == 0);

	CS4500_ASSERT_TRUE(sizeof(ser1.get_buff()) == 8);
	CS4500_ASSERT_TRUE(sizeof(ser2->get_buff()) == 8);

	delete ser2;

	exit(0);
}

TEST(SerializerTests, serializerConstructorAndGetter) {ASSERT_EXIT_ZERO(serializerConstructorAndGetter)}


void serializerSerialize() {
	Serializer ser = Serializer();

	bool demoBool = true;
	ser.serialize_bool(demoBool);
	ser.clear();

	int demoInt = 12345;
	ser.serialize_int(demoInt);
	ser.clear();

	double demoDouble = 3.14159265358979323846264338327950288419716939937510;
	ser.serialize_double(demoDouble);
	ser.clear();

	String* demoString = new String("I'm part of the demo.");
	ser.serialize_string(demoString);
	ser.clear();
	delete demoString;

	StringArray* demoStringArray = new StringArray();
	String* demoStringMember1 = new String("hello");
	String* demoStringMember2 = new String("world");
	String* demoStringMember3 = new String("amigo");
	demoStringArray->pushBack(demoStringMember1);
	demoStringArray->pushBack(demoStringMember2);
	demoStringArray->pushBack(demoStringMember3);
	ser.serialize_string_array(demoStringArray);
	ser.clear();
	delete demoStringArray;

	String* ip0 = new String("127.0.0.0");
	int port0 = 8080;
	NodeInfo* ni = new NodeInfo(ip0, port0);
	ser.serialize_node_info(ni);
	ser.clear();
	delete ni;

	String* ip1 = new String("127.0.0.1");
	int port1 = 8081;
	String* ip2 = new String("127.0.0.2");
	int port2 = 8182;
	NodeInfo* ni1 = new NodeInfo(ip1, port1);
	NodeInfo* ni2 = new NodeInfo(ip2, port2);
	NodeInfoArray* nia = new NodeInfoArray();
	nia->pushBack(ni1);
	nia->pushBack(ni2);
	ser.serialize_node_info_array(nia);
	ser.clear();
	delete nia;

	exit(0);
}

TEST(SerializerTests, serializerSerialize) {ASSERT_EXIT_ZERO(serializerSerialize)}

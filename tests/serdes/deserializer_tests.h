#include <gtest/gtest.h>


void deserializerConstructor() {
	Deserializer des = Deserializer();

	exit(0);
}

TEST(DeserializerTests, deserializerConstructor) {ASSERT_EXIT_ZERO(deserializerConstructor)}


void deserializerDeserialize() {
	Serializer s = Serializer();
	Deserializer d = Deserializer();

	bool demo_bool = true;
	s.serialize_bool(demo_bool);
	bool result_bool = d.deserialize_bool(s.get_buff());
	ASSERT_TRUE(demo_bool == result_bool);
	s.clear();

	int demo_int = 12345;
	s.serialize_int(demo_int);
	int result_int = d.deserialize_int(s.get_buff());
	ASSERT_TRUE(demo_int == result_int);
	s.clear();

	double demoDouble = 3.14159265358979323846264338327950288419716939937510;
	s.serialize_double(demoDouble);
	double resultDouble = d.deserialize_double(s.get_buff());
	ASSERT_TRUE(resultDouble + 0.000000001 > demoDouble);
	ASSERT_TRUE(resultDouble - 0.000000001 < demoDouble);
	s.clear();

	String* demo_string = new String("hello, world");
	s.serialize_string(demo_string);
	String* result_string = d.deserialize_string(s.get_buff());
	assert (demo_string->equals(result_string));
	assert (result_string->equals(demo_string));
	delete demo_string;
	delete result_string;
	s.clear();

	StringArray* demo_string_array = new StringArray();
	String* demo_string_member1 = new String("hello");
	String* demo_string_member2 = new String("world");
	String* demo_string_member3 = new String("amigo");
	demo_string_array->pushBack(demo_string_member1);
	demo_string_array->pushBack(demo_string_member2);
	demo_string_array->pushBack(demo_string_member3);
	s.serialize_string_array(demo_string_array);
	StringArray* result_string_array = d.deserialize_string_array(s.get_buff(), 3);
	// These String* borrows
	String* result_string_member1 = result_string_array->get(0);
	String* result_string_member2 = result_string_array->get(1);
	String* result_string_member3 = result_string_array->get(2);
	assert (demo_string_member1->equals(result_string_member1));
	assert (demo_string_member2->equals(result_string_member2));
	assert (demo_string_member3->equals(result_string_member3));
	delete demo_string_array;
	delete result_string_array;
	s.clear();

	String* ip = new String("127.0.0.1");
	int port = 8080;
	NodeInfo* demo_node_info = new NodeInfo(ip, port);
	s.serialize_node_info(demo_node_info);
	NodeInfo* result_node_info = d.deserialize_node_info(s.get_buff());
	assert (demo_node_info->to_string()->equals(result_node_info->to_string()));
	delete demo_node_info;
	delete result_node_info;
	s.clear();

	String* ip1 = new String("127.0.0.1");
	int port1 = 8080;
	String* ip2 = new String("127.0.0.2");
	int port2 = 8181;
	NodeInfo* demo_node_info1 = new NodeInfo(ip1, port1);
	NodeInfo* demo_node_info2 = new NodeInfo(ip2, port2);
	NodeInfoArray* demo_node_info_array = new NodeInfoArray();
	demo_node_info_array->pushBack(demo_node_info1);
	demo_node_info_array->pushBack(demo_node_info2);
	s.serialize_node_info_array(demo_node_info_array);
	NodeInfoArray* result_node_info_array = d.deserialize_node_info_array(s.get_buff(), 2);
	assert (result_node_info_array->get(0)->to_string()->equals(demo_node_info1->to_string()));
	assert (result_node_info_array->get(1)->to_string()->equals(demo_node_info2->to_string()));
	delete demo_node_info_array;
	delete result_node_info_array;
	s.clear();

	exit(0);
}

TEST(DeserializerTests, deserializerDeserialize) {ASSERT_EXIT_ZERO(deserializerDeserialize)}

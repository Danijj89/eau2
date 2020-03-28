//lang::CwC


/**
 * Main: This function shows off the capabilities of the serializer
 * and deserializer, and demonstrate suggested uses.
 *
 * Author: zhan.d@husky.neu.edu, cao.yuan1@husky.neu.edu
 */


#include <cassert>


#include "serializer.h"
#include "deserializer.h"


int main() {
	Serializer s = Serializer();
	Deserializer d = Deserializer();

	// Demonstrate the ability to serialize a bool
	bool demo_bool = true;
	s.serialize_bool(demo_bool);
	bool result_bool = d.deserialize_bool(s.get_buff());
	assert (demo_bool == result_bool);
	s.clear();

	// size_t
	size_t demo_size_t = 12345;
	s.serialize_int(demo_size_t);
	size_t result_size_t = d.deserialize_size_t(s.get_buff());
	assert (demo_size_t == result_size_t);
	s.clear();

	// Demonstrate the ability to serialize an int
	int demo_int = -12345;
	s.serialize_int(demo_int);
	int result_int = d.deserialize_int(s.get_buff());
	assert (demo_int == result_int);
	s.clear();


	// Demonstrate the ability to serialize a float
	float demo_float = 3.14159;
	s.serialize_double(demo_float);
	float result_float = d.deserialize_double(s.get_buff());
	assert (demo_float > result_float - 0.001);
	assert (demo_float < result_float + 0.001);
	s.clear();

	// Demonstrate the ability to serialize a String
	String* demo_string = new String("hello, world");
	s.serialize_string(demo_string);
	String* result_string = d.deserialize_string(s.get_buff());
	assert (demo_string->equals(result_string));
	assert (result_string->equals(demo_string));
	delete demo_string;
	delete result_string;
	s.clear();

	// Demonstrate the ability to serialize a StringArray
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

	// Demonstrate the ability to serialize a NodeInfo
	String* ip = new String("127.0.0.1");
	int port = 8080;
	NodeInfo* demo_node_info = new NodeInfo(ip, port);
	s.serialize_node_info(demo_node_info);
	NodeInfo* result_node_info = d.deserialize_node_info(s.get_buff());
	assert (demo_node_info->to_string()->equals(result_node_info->to_string()));
	delete demo_node_info;
	delete result_node_info;
	s.clear();

	// Demonstrate the ability to serialize a NodeInfoArray
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


	return 0;
}

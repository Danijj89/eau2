#include "../../src/network/message.h"
#include "../../src/network/msg_kind.h"
#include "../../src/serialization/data_type.h"


void messageConstructorDestructorTest() {
	String* s = new String("some message");
	Message m1 = Message(MsgKind::PUT, DataType::KEY, s);
	Message* m2 = new Message(MsgKind::REPLY, DataType::PAIR, s);
	Message m3 = Message(MsgKind::GET, DataType::VALUE);
	Message* m4 = new Message(MsgKind::DIRECTORY, DataType::NODE_INFO_ARRAY);
	Message m5 = Message(MsgKind::SHUTDOWN, DataType::EMPTY);
	Message* m6 = new Message(MsgKind::ADD_KEY, DataType::KEY);

	delete s;
	delete m2;
	delete m4;
	delete m6;

	exit(0);
}

TEST(MessageTests, messageConstructorDestructorTest) {ASSERT_EXIT_ZERO(messageConstructorDestructorTest)}


void messageGetterTest() {
	String* s = new String("some message");
	Message m1 = Message(MsgKind::PUT, DataType::KEY, s);

	ASSERT_EQUALS(m1.getKind(), MsgKind::PUT);
	ASSERT_EQUALS(m1.getType(), DataType::KEY);
	ASSERT_EQUALS(strcmp(m1.getBody(), "some message"), 0);
	ASSERT_EQUALS(m1.getSize(), strlen("some message"));

	delete s;

	exit(0);
}

TEST(MessageTests, messageGetterTest) {ASSERT_EXIT_ZERO(messageGetterTest)}


void messageAddBodyTest() {
	String* s1 = new String("some message");
	Message m1 = Message(MsgKind::PUT, DataType::KEY, s1);

	ASSERT_EQUALS(m1.getKind(), MsgKind::PUT);
	ASSERT_EQUALS(m1.getType(), DataType::KEY);
	ASSERT_EQUALS(strcmp(m1.getBody(), "some message"), 0);
	ASSERT_EQUALS(m1.getSize(), strlen("some message"));

	String* s2 = new String("another message");

	m1.addBody(s2->c_str(), s2->size());
	ASSERT_EQUALS(m1.getKind(), MsgKind::PUT);
	ASSERT_EQUALS(m1.getType(), DataType::KEY);
	ASSERT_EQUALS(strcmp(m1.getBody(), "another message"), 0);
	ASSERT_EQUALS(m1.getSize(), strlen("another message"));

	delete s1;
	delete s2;

	exit(0);
}

TEST(MessageTests, messageAddBodyTest) {ASSERT_EXIT_ZERO(messageAddBodyTest)}
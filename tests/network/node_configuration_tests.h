#include "../../src/network/node_configuration.h"


void nodeConfConstructorDestructor() {
	NodeConfiguration conf1 = NodeConfiguration();
	NodeConfiguration* conf2 = new NodeConfiguration();

	delete conf2;

	exit(0);
}

TEST(NodeConfigurationTests, nodeConfConstructorDestructor) {ASSERT_EXIT_ZERO(nodeConfConstructorDestructor)}


void nodeConfSetterGetterTest() {
	NodeConfiguration conf = NodeConfiguration();

	String* inputIP = new String("127.0.0.1");
	conf.setIP(inputIP);
	String* IP = conf.getIP();
	ASSERT_EQ(inputIP, IP); // pointer equality


	int inputPort = 12345;
	conf.setPort(inputPort);
	ASSERT_EQ(inputPort, conf.getPort());

	size_t numNodes = 10;
	conf.setNumNodes(numNodes);
	ASSERT_EQ(numNodes, conf.getNumNodes());

	int sockBacklog = 5;
	conf.setSocketBacklog(sockBacklog);
	ASSERT_EQ(sockBacklog, conf.getSocketBacklog());

	String* serverIP = new String("127.0.0.1");
	conf.setServerIP(serverIP);
	String* gotServerIP = conf.getServerIP();
	ASSERT_EQ(gotServerIP, serverIP); // pointer equality

	conf.setServerPort(inputPort);
	ASSERT_EQ(inputPort, conf.getServerPort());


	ASSERT_TRUE(conf.getIsServer());
	conf.setServerPort(inputPort - 1);
	ASSERT_FALSE(conf.getIsServer());
	conf.setServerPort(inputPort);
	ASSERT_TRUE(conf.getIsServer());
	conf.setServerIP(new String("some ip"));
	ASSERT_FALSE(conf.getIsServer());
	String* badIP = conf.getServerIP();

	char* path = const_cast<char*>("/home/user/eau2/somefile.txt");
	conf.setFilePath(path);
	char* gotPath = conf.getFilePath();
	ASSERT_TRUE(path == gotPath); // pointer equality

	size_t n = 1024;
	conf.setChunkItems(n);
	ASSERT_EQ(n, conf.getChunkItems());

	delete IP;
	delete serverIP;
	delete badIP;

	exit(0);
}

TEST(NodeConfigurationTests, nodeConfSetterGetterTest) {ASSERT_EXIT_ZERO(nodeConfSetterGetterTest)}


void nodeConfOwnershipTest() {
	NodeConfiguration* conf = new NodeConfiguration();

	String* inputIP = new String("127.0.0.1");
	conf->setIP(inputIP);

	delete conf; // deleting conf first should not delete inputIP
	delete inputIP;

	exit(0);
}

TEST(NodeConfigurationTests, nodeConfOwnershipTest) {ASSERT_EXIT_ZERO(nodeConfOwnershipTest)}

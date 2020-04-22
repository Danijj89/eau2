#include "../../src/util/arg_parser.h"
#include "../../src/network/node_configuration.h"


void parserTest() {
	int argc = 17;
	char** argv = new char*[17];
	argv[0] = const_cast<char*>("main");
	argv[1] = const_cast<char*>("-ip");
	argv[2] = const_cast<char*>("127.0.0.1");
	argv[3] = const_cast<char*>("-port");
	argv[4] = const_cast<char*>("9000");
	argv[5] = const_cast<char*>("-server_ip");
	argv[6] = const_cast<char*>("127.0.0.1");
	argv[7] = const_cast<char*>("-server_port");
	argv[8] = const_cast<char*>("9000");
	argv[9] = const_cast<char*>("-nodes");
	argv[10] = const_cast<char*>("10");
	argv[11] = const_cast<char*>("-socket_backlog");
	argv[12] = const_cast<char*>("5");
	argv[13] = const_cast<char*>("-file");
	argv[14] = const_cast<char*>("file.txt");
	argv[15] = const_cast<char*>("-chunk_items");
	argv[16] = const_cast<char*>("1000");

	ArgParser parser = ArgParser();

	NodeConfiguration* conf = parser.parse(argc, argv);

	String* IP = conf->getIP();
	int port = conf->getPort();
	size_t numNodes = conf->getNumNodes();
	int backlog = conf->getSocketBacklog();
	String* serverIP = conf->getServerIP();
	int serverPort = conf->getServerPort();
	char* path = conf->getFilePath();
	size_t chunkItems = conf->getChunkItems();

	ASSERT_TRUE(IP->equals(serverIP));
	ASSERT_TRUE(port == 9000);
	ASSERT_TRUE(serverPort = port);
	ASSERT_TRUE(conf->getIsServer());
	ASSERT_TRUE(numNodes == 10);
	ASSERT_TRUE(backlog = 5);
	ASSERT_TRUE(chunkItems == 1000);
	ASSERT_TRUE(strcmp(path, argv[14]) == 0);

	delete IP;
	delete serverIP;
	delete[] argv;

	delete conf;

	exit(0);
}

TEST(ArgParserTests, parserTest) {ASSERT_EXIT_ZERO(parserTest)}


void outOfOrderOkayrTest() {
	int argc = 17;
	char** argv = new char*[17];
	argv[0] = const_cast<char*>("main");
	argv[5] = const_cast<char*>("-ip");
	argv[6] = const_cast<char*>("127.0.0.1");
	argv[3] = const_cast<char*>("-port");
	argv[4] = const_cast<char*>("9000");
	argv[1] = const_cast<char*>("-server_ip");
	argv[2] = const_cast<char*>("127.0.0.1");
	argv[7] = const_cast<char*>("-server_port");
	argv[8] = const_cast<char*>("9000");
	argv[11] = const_cast<char*>("-nodes");
	argv[12] = const_cast<char*>("10");
	argv[9] = const_cast<char*>("-socket_backlog");
	argv[10] = const_cast<char*>("5");
	argv[15] = const_cast<char*>("-file");
	argv[16] = const_cast<char*>("file.txt");
	argv[13] = const_cast<char*>("-chunk_items");
	argv[14] = const_cast<char*>("1000");

	ArgParser parser = ArgParser();

	NodeConfiguration* conf = parser.parse(argc, argv);

	String* IP = conf->getIP();
	int port = conf->getPort();
	size_t numNodes = conf->getNumNodes();
	int backlog = conf->getSocketBacklog();
	String* serverIP = conf->getServerIP();
	int serverPort = conf->getServerPort();
	char* path = conf->getFilePath();
	size_t chunkItems = conf->getChunkItems();

	ASSERT_TRUE(IP->equals(serverIP));
	ASSERT_TRUE(port == 9000);
	ASSERT_TRUE(serverPort = port);
	ASSERT_TRUE(conf->getIsServer());
	ASSERT_TRUE(numNodes == 10);
	ASSERT_TRUE(backlog = 5);
	ASSERT_TRUE(chunkItems == 1000);
	ASSERT_TRUE(strcmp(path, argv[16]) == 0);

	delete IP;
	delete serverIP;
	delete[] argv;

	delete conf;

	exit(0);
}

TEST(ArgParserTests, outOfOrderOkayrTest) {ASSERT_EXIT_ZERO(outOfOrderOkayrTest)}


void tooFewArgumentsTest() {
	int argc = 15;
	char** argv = new char*[15];
	argv[0] = const_cast<char*>("main");
	argv[1] = const_cast<char*>("-ip");
	argv[2] = const_cast<char*>("127.0.0.1");
	argv[3] = const_cast<char*>("-port");
	argv[4] = const_cast<char*>("9000");
	argv[5] = const_cast<char*>("-server_ip");
	argv[6] = const_cast<char*>("127.0.0.1");
	argv[7] = const_cast<char*>("-server_port");
	argv[8] = const_cast<char*>("9000");
	argv[9] = const_cast<char*>("-nodes");
	argv[10] = const_cast<char*>("10");
	argv[11] = const_cast<char*>("-socket_backlog");
	argv[12] = const_cast<char*>("5");
	argv[13] = const_cast<char*>("-file");
	argv[14] = const_cast<char*>("file.txt");

	ArgParser parser = ArgParser();

	NodeConfiguration* conf = parser.parse(argc, argv);
}

TEST(ArgParserTests, tooFewArgumentsTest) {ASSERT_SIGNAL_SIX(tooFewArgumentsTest)}


void tooManyArgumentsTest() {
	int argc = 19;
	char** argv = new char*[19];
	argv[0] = const_cast<char*>("main");
	argv[1] = const_cast<char*>("-ip");
	argv[2] = const_cast<char*>("127.0.0.1");
	argv[3] = const_cast<char*>("-port");
	argv[4] = const_cast<char*>("9000");
	argv[5] = const_cast<char*>("-server_ip");
	argv[6] = const_cast<char*>("127.0.0.1");
	argv[7] = const_cast<char*>("-server_port");
	argv[8] = const_cast<char*>("9000");
	argv[9] = const_cast<char*>("-nodes");
	argv[10] = const_cast<char*>("10");
	argv[11] = const_cast<char*>("-socket_backlog");
	argv[12] = const_cast<char*>("5");
	argv[13] = const_cast<char*>("-file");
	argv[14] = const_cast<char*>("file.txt");
	argv[15] = const_cast<char*>("-chunk_items");
	argv[16] = const_cast<char*>("1000");
	argv[17] = const_cast<char*>("-ip");
	argv[18] = const_cast<char*>("127.0.0.1");

	ArgParser parser = ArgParser();

	NodeConfiguration* conf = parser.parse(argc, argv);
}

TEST(ArgParserTests, tooManyArgumentsTest) {ASSERT_SIGNAL_SIX(tooManyArgumentsTest)}


void invertOptionAndParamTest() {
	int argc = 17;
	char** argv = new char*[17];
	argv[0] = const_cast<char*>("main");
	argv[2] = const_cast<char*>("-ip"); // Inverted
	argv[1] = const_cast<char*>("127.0.0.1"); // Inverted
	argv[3] = const_cast<char*>("-port");
	argv[4] = const_cast<char*>("9000");
	argv[5] = const_cast<char*>("-server_ip");
	argv[6] = const_cast<char*>("127.0.0.1");
	argv[7] = const_cast<char*>("-server_port");
	argv[8] = const_cast<char*>("9000");
	argv[9] = const_cast<char*>("-nodes");
	argv[10] = const_cast<char*>("10");
	argv[11] = const_cast<char*>("-socket_backlog");
	argv[12] = const_cast<char*>("5");
	argv[13] = const_cast<char*>("-file");
	argv[14] = const_cast<char*>("file.txt");
	argv[15] = const_cast<char*>("-chunk_items");
	argv[16] = const_cast<char*>("1000");

	ArgParser parser = ArgParser();

	NodeConfiguration* conf = parser.parse(argc, argv);
}

TEST(ArgParserTests, invertOptionAndParamTest) {ASSERT_SIGNAL_SIX(invertOptionAndParamTest)}

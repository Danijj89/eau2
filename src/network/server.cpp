#include <iostream>

#include "input_parser.h"
#include "server.h"

int main(int argc, char** argv) {

	InputParser* p = new InputParser();
	p->parse_input(argc, argv);
	Server* server = new Server(p->get_ip()->clone(), p->get_port());
	server->start();
	delete p;
	delete server;
	return 0;
}

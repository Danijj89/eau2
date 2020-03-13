

#include "input_parser.h"
#include "node.h"

int main(int argc, char** argv) {
	/*
	Client* c = new Client(p->get_ip(), p->get_port());
	c->connect_to_node(const_cast<char*>("127.0.0.1"));
	*/
	/*
	c->send_message(c->get_node_socket(), const_cast<char*>("Hi this is client"));
	while (true) {
		c->read_message(c->get_node_socket());
	}
	*/

	// Goal
	InputParser* p = new InputParser();
	p->parse_input(argc, argv);
	Node* client = new Node(p->get_ip(), p->get_port());
	client->start();
	delete p;
	delete client;

	return 0;
}

#include <stdio.h>
#include <iostream>


#include "../util/string.h"


#include "kernode.h"

int main(int argc, char** argv) {
	String* ip = new String(argv[1]); // will be stolen by NodeInfo in Kernode
	int port = atoi(argv[2]);

	Kernode* k = new Kernode(ip, port);
	k->run();

	delete k;

	return 0;
}
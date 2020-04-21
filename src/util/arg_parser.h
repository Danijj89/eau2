#pragma once

#include "object.h"
#include "../network/node_configuration.h"
#include "helper.h"

class ArgParser : public Object {
public:

	static NodeConfiguration* parse(int argc, char** argv) {
		NodeConfiguration* conf = new NodeConfiguration();
		// Make sure that the number of arguments given are in the correct range
		assert(argc == 17);
		int i = 1;
		while (i < argc) {
			char* flag = argv[i];
			char* val = argv[i + 1];
			if (strcmp(flag, "-ip") == 0) {
				conf->setIP(new String(val));
				i += 2;
			} else if (strcmp(flag, "-port") == 0) {
				conf->setPort(atoi(val));
				i += 2;
			} else if (strcmp(flag, "-server_ip") == 0) {
				conf->setServerIP(new String(val));
				i += 2;
			} else if (strcmp(flag, "-server_port") == 0) {
				conf->setServerPort(atoi(val));
				i += 2;
			} else if (strcmp(flag, "-nodes") == 0) {
				conf->setNumNodes(parse_uint(val));
				i += 2;
			} else if (strcmp(flag, "-socket_backlog") == 0) {
				conf->setSocketBacklog(atoi(val));
				i += 2;
			} else if (strcmp(flag, "-file") == 0) {
				conf->setFilePath(val);
				i += 2;
			} else if (strcmp(flag, "-chuck_items") == 0) {
				conf->setChunkItems(parse_uint(val));
				i += 2;
			} else {
				assert(false);
			}
		}
		return conf;
	}
};
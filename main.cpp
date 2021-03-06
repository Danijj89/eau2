#include <stdio.h>

#include "src/network/node_configuration.h"
#include "src/util/arg_parser.h"
#include "src/application/wordcount/wordcount.h"

int main(int argc, char** argv) {
	NodeConfiguration* conf = ArgParser::parse(argc, argv);
	WordCount app = WordCount(conf);
	app.run();
	app.waitShutDown();

	delete conf;

	printf("Run terminated\n");
	return 0;
}

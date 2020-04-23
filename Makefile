DOCKER := docker run -ti -v `pwd`:/test w2-gtest:0.1 bash -c
CXXFLAGS := --std=c++11 -Wall --pedantic -O3 -pthread
COMPILER := g++
NETWORK_CONFIGURATIONS := -nodes 2 -socket_backlog 5 -chunk_items 100
SERVER := -ip 127.0.0.1 -port 9000 -server_ip 127.0.0.1 -server_port 9000
CLIENT := -ip 127.0.0.1 -port 9001 -server_ip 127.0.0.1 -server_port 9000
FILE := -file 100k.txt

run: docker_run

build:
	$(COMPILER) $(CXXFLAGS) main.cpp -o main

server: build
	./main $(SERVER) $(NETWORK_CONFIGURATIONS) $(FILE)

client: build
	./main  $(CLIENT) $(NETWORK_CONFIGURATIONS) $(FILE)

docker_build:
	$(DOCKER) "cd /test; $(COMPILER) $(CXXFLAGS) main.cpp -o main"

docker_run: docker_build
	$(DOCKER) "cd /test; ./main"

docker_server: docker_build
	$(DOCKER) "$(SERVER) $(NETWORK_CONFIGURATIONS) $(FILE)"

docker_client: docker_build
	$(DOCKER) "$(CLIENT) $(NETWORK_CONFIGURATIONS) $(FILE)"

docker_valgrind: docker_build
	$(DOCKER) "cd /test; valgrind --leak-check=full ./main"

test_setup:
	$(DOCKER) "cd /test/tests; cmake ."

unittest: test_setup
	$(DOCKER) "cd /test/tests; make; ./runTests"

functionaltest: test_setup
	$(DOCKER) "cd /test/tests; make; ./nodeTest"

valgrind: test_setup
	$(DOCKER) "cd /test/tests; make; valgrind ./runTests"

clean:
	sudo git clean -df

.PHONY: sorer clean

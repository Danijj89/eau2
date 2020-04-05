DOCKER := docker run -ti -v `pwd`:/test w2-gtest:0.1 bash -c
CXXFLAGS := --std=c++11 -Wall --pedantic -O3 -pthread
COMPILER := g++

network:
	$(DOCKER) "cd /test/src/network; $(COMPILER) $(CSSFLAGS) kernode.cpp -o kernode && ./kernode"

local:
	cd sorer; $(COMPILER) $(CXXFLAGS) main.cpp -o main

docker_build:
	$(DOCKER) "cd /test; $(COMPILER) $(CXXFLAGS) main.cpp -o main"

docker_run: docker_build
	$(DOCKER) "cd /test; ./main"

docker_valgrind: docker_build
	$(DOCKER) "cd /test; valgrind --leak-check=full ./main"

test_setup:
	$(DOCKER) "cd /test/tests; cmake ."

test: test_setup
	$(DOCKER) "cd /test/tests; make && ./runTests"

valgrind: test_setup
	$(DOCKER) "cd /test/tests; valgrind ./runTests"

clean:
	sudo git clean -df

.PHONY: sorer clean

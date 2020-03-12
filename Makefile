DOCKER := docker run -ti -v `pwd`:/test w2-gtest:0.1 bash -c
CXXFLAGS := --std=c++11 -Wall --pedantic -O3
COMPILER := g++


local:
	cd sorer; $(COMPILER) $(CXXFLAGS) main.cpp -o sorer


docker:
	$(DOCKER) "cd /test/sorer; $(COMPILER) $(CXXFLAGS) main.cpp -o sorer"


test_setup:
	$(DOCKER) "cd /test/tests; cmake ."

test: test_setup
	$(DOCKER) "cd /test/tests; make && ./runTests"

valgrind: test
	$(DOCKER) "cd /test/tests; valgrind ./runTests"

clean:
	sudo git clean -df

.PHONY: sorer clean

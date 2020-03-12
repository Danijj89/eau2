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

test_sorer:
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/0.sor -print_col_type 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/0.sor -is_missing_idx 0 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -print_col_type 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -is_missing_idx 0 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -print_col_idx 0 3"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_type 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_type 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_type 2"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_type 3"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -is_missing_idx 1 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -is_missing_idx 1 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_idx 3 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_idx 3 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_idx 2 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/2.sor -print_col_idx 2 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/3.sor -print_col_type 4"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/3.sor -print_col_idx 2 10"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/3.sor -print_col_idx 2 384200"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/4.sor -print_col_idx 0 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/4.sor -print_col_idx 0 2"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/4.sor -print_col_idx 1 1"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/4.sor -print_col_idx 1 2"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -from 1 -len 74 -print_col_type 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -from 1 -len 74 -is_missing_idx 0 0"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/1.sor -from 1 -len 74 -print_col_idx 0 6"
#	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/7.sor -print_col_idx 1 400"
	- $(DOCKER) "cd /test/sorer; ./sorer -f ../tests/sorer/100mb.sor -print_col_idx 1 400"

clean:
	sudo git clean -df

.PHONY: sorer clean

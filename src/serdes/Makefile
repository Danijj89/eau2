CXXFLAGS := --std=c++11 -Wall --pedantic -ggdb3

build:
	@ c++ $(CXXFLAGS) main.cpp -o demo

demo: build
	@ ./demo

valgrind: build
	@ valgrind ./demo
	@ echo ""

run: build valgrind
	@ ./demo
	@ echo "Silence is golden."

clean:
	rm demo

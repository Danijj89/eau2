CXXFLAGS := --std=c++11 -Wall --pedantic -pthread

build:
	g++ $(CXXFLAGS) server.cpp -o server
	g++ $(CXXFLAGS) client.cpp -o client
	g++ $(CXXFLAGS) kernode.cpp -o kernode

start_server: server
	./server -ip 127.0.0.1:9000

start_client1: client
	./client -ip 127.0.0.2:9000

start_client2: client
	./client -ip 127.0.0.3:9000

run:
	./server -ip 127.0.0.1:9000 &
	sleep 2
	./client -ip 127.0.0.2:9000 &
	sleep 2
	./client -ip 127.0.0.3:9000

clean:
	rm client server
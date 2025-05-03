CXX = clang++
CXXFLAGS = -std=c++17 -I$(IX)/ -I$(IX)/ixwebsocket
LDFLAGS = -L$(IX)/build -lixwebsocket -lssl -lcrypto -lpthread -lz

IX = $(HOME)/IXWebSocket

all: client server

client: main.cpp
	$(CXX) $(CXXFLAGS) new_main.cpp -o client $(LDFLAGS)

server: servergit.cpp
	$(CXX) $(CXXFLAGS) server.cpp -o server $(LDFLAGS)

clean:
	rm -f client server


CPP_FLAGS = -Wall -Werror -std=c++11

all: server/server client/client

server/server_socket.o : server/server_socket.cpp server/server_socket.hpp
	g++ $(CPP_FLAGS) -c server/server_socket.cpp -o $@

server/text_handlers.o : server/text_handlers.cpp server/text_handlers.hpp 
	g++ $(CPP_FLAGS) -c server/text_handlers.cpp -o $@

server/server.o: server/server.cpp server/text_handlers.hpp server/server_socket.hpp
	g++ $(CPP_FLAGS) -c server/server.cpp -o $@

server/server: server/text_handlers.o server/server_socket.o server/server.o 
	g++ $(CPP_FLAGS) $^ -o $@

client/client_socket.o : client/client_socket.cpp client/client_socket.hpp
	g++ $(CPP_FLAGS) -c client/client_socket.cpp -o $@

client/client.o: client/client.cpp client/client_socket.hpp
	g++ $(CPP_FLAGS) -c client/client.cpp -o $@

client/client: client/client.o client/client_socket.o
	g++ $(CPP_FLAGS) $^ -o $@

clean:
	rm -f *.o

.PHONY: clean all

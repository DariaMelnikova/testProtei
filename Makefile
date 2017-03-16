CPP_FLAGS = -Wall -Werror -std=c++11

all: server/server client/client

server/text_handlers.o : server/text_handlers.cpp server/text_handlers.hpp
	g++ $(CPP_FLAGS) -c server/text_handlers.cpp -o $@

server/server.o: server/server.cpp server/text_handlers.hpp
	g++ $(CPP_FLAGS) -c server/server.cpp -o $@

server/server: server/text_handlers.o server/server.o 
	g++ $(CPP_FLAGS) $^ -o $@

client/client: client/client.cpp
	g++ $(CPP_FLAGS) $^ -o $@

clean:
	rm -f *.o

.PHONY: clean all

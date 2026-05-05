CC = gcc
CXX = g++

RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

CFLAGS = -Wall -Wextra -I./shared -g -pthread
CXXFLAGS = -Wall -Wextra -I./shared -g -pthread -std=c++17

SERVER_SRCS = $(wildcard server/*.c)
CLIENT_SRCS = $(wildcard client/*.cpp)
SHARED_SRCS = $(wildcard shared/*.cpp) 

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
SHARED_OBJS = $(SHARED_SRCS:.cpp=.o)

SERVER_BIN = bin/server
CLIENT_BIN = bin/client

all: directories $(SERVER_BIN) $(CLIENT_BIN)

directories:
	mkdir -p bin

$(SERVER_BIN): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT_BIN): $(CLIENT_OBJS) $(SHARED_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(RAYLIB_FLAGS)

server/%.o: server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

client/%.o: client/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

shared/%.o: shared/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f server/*.o client/*.o shared/*.o
	rm -rf bin/

.PHONY: all directories clean
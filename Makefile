# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

LIB_DIR=library
SERVER_DIR=server
CLIENT_DIR=client


all:
	$(MAKE) -C $(LIB_DIR) all
	$(MAKE) -C $(SERVER_DIR) all
	$(MAKE) -C $(CLIENT_DIR) all

.PHONY: all clean server client library

library:
	$(MAKE) -C $(LIB_DIR) libclientserver.a

server:
	$(MAKE) -C $(SERVER_DIR) newsserver

client:
	$(MAKE) -C $(CLIENT_DIR) myclient

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(SERVER_DIR) clean
	$(MAKE) -C $(CLIENT_DIR) clean
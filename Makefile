CC = g++

ifeq ($(config),)
config = debug
endif

ifeq ($(config), debug)
CFLAGS = -O3 -w -std=c++17
else
CFLAGS = -g -Wall -Wextra -std=c++17
endif

INC = -Iinclude

SRC = $(wildcard src/*.cpp) $(wildcard src/util/*.cpp)

all: pch
	$(CC) $(CFLAGS) $(INC) $(SRC) -o bin/CFGR

pch: include/pch.h
	$(CC) -c $^ $(CFLAGS) -o include/pch.h.gch

CC:=gcc
CPP:=g++
BCS_FLAGS:=-lbcs_sdk -lcrypto -lcurl
FLAGS:=$(shell pkg-config fuse --cflags --libs) -DFUSE_USE_VERSION=22

all: 
	$(CC) -g $(FLAGS) netdisk.c -o netdisk

test: bcs-api.o bcs-api-test.cc
	$(CPP) -g $(BCS_FLAGS) bcs-api.o bcs-api-test.cc -o test

bcs-api.o: bcs-api.h bcs-api.cc
	$(CPP) -c -g $(BCS_FLAGS) bcs-api.cc -o bcs-api.o

clean:
	rm -r *.o test netdisk

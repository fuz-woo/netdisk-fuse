CC:=clang
CPP:=clang
LINK:=g++

BCS_FLAGS:=-lbcs_sdk -lcrypto -lcurl

FLAGS:=$(shell pkg-config fuse --cflags --libs) -DFUSE_USE_VERSION=22 -D_FILE_OFFSET_BITS=64

all: bcs-api.o netdisk.o main.o
	$(LINK) $(BCS_FLAGS) $(FLAGS) bcs-api.o netdisk.o main.o -o netdisk

main.o: main.cc
	$(CPP) $(BCS_FLAGS) $(FLAGS) -c main.cc -o main.o

netdisk.o: netdisk.h netdisk.cc
	$(CPP) -c $(FLAGS) netdisk.cc -o netdisk.o

bcs-api.o: bcs-api.h bcs-api.cc
	$(CPP) -c $(BCS_FLAGS) bcs-api.cc -o bcs-api.o


clean:
	rm -r *.o test netdisk

test: bcs-api.o bcs-api-test.cc
	$(CPP) -g $(BCS_FLAGS) bcs-api.o bcs-api-test.cc -o test


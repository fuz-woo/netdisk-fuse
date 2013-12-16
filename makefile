CPP:=clang++
LINK:=clang++

BCS_FLAGS:=-lbcs_sdk -lcrypto -lcurl
CFLAGS:=$(shell pkg-config fuse --cflags) -DFUSE_USE_VERSION=26
FUSE_FLAGS:=$(shell pkg-config fuse --libs)

all: bcs-api.o netdisk.o main.o
	$(CPP) $(CFLAGS) $(BCS_FLAGS) $(FUSE_FLAGS) bcs-api.o netdisk.o main.o -o netdisk

main.o: main.cc
	$(CPP) -c $(CFLAGS) main.cc -o main.o

netdisk.o: netdisk.h netdisk.cc
	$(CPP) -c $(CFLAGS) netdisk.cc -o netdisk.o

bcs-api.o: bcs-api.h bcs-api.cc
	$(CPP) -c bcs-api.cc -o bcs-api.o


clean:
	rm -r *.o test netdisk

test: bcs-api.o bcs-api-test.cc
	$(CPP) -g $(BCS_FLAGS) bcs-api.o bcs-api-test.cc -o test


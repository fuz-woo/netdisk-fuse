CC:=gcc
FLAGS:=$(shell pkg-config fuse --cflags --libs) -DFUSE_USE_VERSION=22

all: 
	$(CC) -g $(FLAGS) netdisk.c -o netdisk

clean:
	rm -r *.o

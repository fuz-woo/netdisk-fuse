#include <fuse.h>
#include <iostream>
using namespace std;

extern struct fuse_operations netdisk_opr;

int
main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &netdisk_opr, NULL);
}

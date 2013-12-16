#include "bcs-api.h"

int
main(int argc, char **argv)
{
	if ( argc < 3) {
		perror("usage: ./test [localfile] [remote_file]\n");
		return -1;
	}

	netdisk_bcs disk(string("reIi2eE41AGGn9zd0FxuTHHY"),
			string("VaiNwPV9tvphCLRvlZ2RvVRkvPHrwaCf"),
			string("netdisk-fuz"));
	
/*	if ( disk.upload(argv[1],argv[1]) != 0 ) {
		perror("Error Uploading...\n");
	} else {
		if ( disk.download(argv[2],argv[1]) != 0 ) {
			perror("Error Downloading...\n");
		} else {
			printf("Done.\n");
		}
		printf("Done.\n");
	}
	*/
	if ( disk.download(argv[2],argv[1]) != 0 ) {
		perror("Error Downloading...\n");
	} else {
		printf("Done.\n");
	}

//	disk.list("netdisk-fuz");
	return 0;
}

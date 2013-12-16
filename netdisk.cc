extern "C" {
	#include <fuse.h>
}
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
using namespace std;


void *
netdisk_init (struct fuse_conn_info *conn)
{
	std::cout<<"Initializing..."<<endl;
	return 0;
}

void
netdisk_destroy (void *private_data)
{
}

int
netdisk_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
	memcpy(buf, "fuz", 3);
	return 3;
}

int
netdisk_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	std::cout<<"Writing..."<<endl;
	return 0;
}

int
netdisk_open(const char *path, struct fuse_file_info *fi)
{
	std::cout<<"Opening..."<<endl;
	return 0;
}

int
netdisk_opendir(const char *path, struct fuse_file_info *fi)
{
	std::cout<<"Opening Dir..."<<endl;
	return 0;
}

int
netdisk_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	std::cout<<"Reading Dir..."<<endl;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, "netdisk", NULL, 0);
	return 0;
}

int
netdisk_getattr(const char *path, struct stat *stbuf)
{
	memset(stbuf, 0, sizeof(struct stat));
	stbuf->st_mode = S_IFDIR | 0x0755;
	stbuf->st_nlink = 1;
	
	return 0;
}



extern struct fuse_operations netdisk_opr = {
	.open = netdisk_open,
	.read = netdisk_read,
	.write = netdisk_write,
	.opendir = netdisk_opendir,
	.readdir = netdisk_readdir,
	.init = netdisk_init,
	.destroy = netdisk_destroy
};

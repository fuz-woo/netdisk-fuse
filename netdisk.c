#include <stdio.h>
#include <fuse.h>

void *
netdisk_init (struct fuse_conn_info *conn)
{
	printf("Hello, Netdisk-Fuse. Initializing...\n");
	return 0;
}

void *
netdisk_destroy (void *private_data)
{
	printf("Hello, Netdisk-Fuse. Destroying...\n");
	return 0;
}

int
netdisk_read(const char *path, char *buf, size_t size, off_t offset,struct fuse_file_info *fi)
{
	printf("Hello, Netdisk-Fuse. Reading...\n");
	return 0;
}

int
netdisk_write(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	printf("Hello, Netdisk-Fuse. Writing...\n");
	return 0;
}

int
netdisk_open(const char *path, struct fuse_file_info *fi)
{
	printf("Hello, Netdisk-Fuse. Opening...\n");
	return 0;
}

int
netdisk_opendir(const char *path, struct fuse_file_info *fi)
{
	printf("Hello, Netdisk-Fuse. Opening...\n");
	return 0;
}

int
netdisk_readir(const char *path, void *buf, fuse_fill_dir_t fill, off_t offset, struct fuse_file_info *fi)
{
	printf("Hello, Netdisk-Fuse. Reading Directory %s...\n", path);
	return 0;
}



struct fuse_operations netdisk_opr = {
	.init = netdisk_init,
	.destroy = netdisk_destroy,
	.open = netdisk_open,
	.read = netdisk_read,
	.write = netdisk_write,
	.opendir = netdisk_opendir,
	.readdir = netdisk_readir,
};


int
main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &netdisk_opr);
}

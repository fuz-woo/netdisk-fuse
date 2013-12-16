#ifndef NETDISK_H
#define NETDISK_H
#include <fuse.h>
#include <stdio.h>

class netdisk_proto {
private:
	enum {
		undefined,
		BAIDU,
		SINA,
		TENCENT,
		KINGSOFT,
		HUAWEI
	} type;
public:
	netdisk_proto():type(undefined) {}
	
	virtual int upload(const char *file) = 0;
	virtual int download(const char *file) = 0;
	virtual int overwrite(const char *file) = 0;
	virtual size_t read(const char *file, char *buf, size_t size) {}
	virtual size_t write(const char *file, char *buf, size_t size) {}
};

#endif

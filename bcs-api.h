#ifndef BCS_SDK_H
#define BCS_SDK_H

#include <iostream>
#include <bcs_sdk.h>
using namespace std;

#define BCS_URL ("bcs.duapp.com")

class netdisk_bcs {
private:
	std::string m_bcs_ak;
	std::string m_bcs_sk;
	std::string m_bcs_host;
	std::string m_bcs_bucket;

	bcs_sdk *m_handler;

	int m_status;

	bool is_exist(const char *file);
	bool is_exist(std::string file);

public:
	netdisk_bcs(const char *ak, const char *sk, const char *bkt);
	netdisk_bcs(std::string ak, std::string sk, std::string bkt);

	inline std::string get_ak() { return m_bcs_ak; }
	inline std::string get_sk() { return m_bcs_sk; }
	inline std::string get_host() { return m_bcs_host; }
	inline int status() { return m_status; }
	inline bcs_sdk *get_handler() { return m_handler;}
	inline std::string get_bucket() { return m_bcs_bucket; }

	bcs_sdk *init_sdk(const char *ak, const char *sk, const char *host);
	bcs_sdk *init_sdk(std::string ak, std::string sk, std::string host);


	// Do some REAL works
	int upload(const char *file, const char *remote_file);
	int upload(std::string file, std::string remote_file);

	int download(const char *file, const char *remote_file);
	int download(std::string file, std::string remote_file);
};

#endif


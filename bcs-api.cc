#include "bcs-api.h" 

netdisk_bcs::netdisk_bcs(std::string ak, std::string sk, std::string bkt)
{
	m_bcs_ak = ak;
	m_bcs_sk = sk;
	m_bcs_bucket = bkt;
	m_bcs_host = std::string(BCS_URL);

	m_handler = init_sdk(m_bcs_ak, m_bcs_sk, m_bcs_host);

	m_status = m_handler==NULL?-1:0;
}

netdisk_bcs::netdisk_bcs(const char *ak, const char *sk, const char *bkt)
{
	new (this)netdisk_bcs(std::string(ak), std::string(sk), std::string(bkt));
}

/*
 * Methods to get user ak and sk and host
 */
/*std::string
netdisk_bcs::get_ak()
{
	return "reIi2eE41AGGn9zd0FxuTHHY";
}

std::string
netdisk_bcs::get_sk()
{
	return "VaiNwPV9tvphCLRvlZ2RvVRkvPHrwaCf";
}

std::string
netdisk_bcs::get_host()
{
	return "bcs.duapp.com";
}
*/

/*
 * Status of bcs service
 */
/*int
netdisk_bcs::status()
{
	if ( get_handler() == NULL ) {
		return -1;
	} else {
		return 0;
	}
}
*/

/*
 * Initialize bcs_sdk
 */
bcs_sdk *
netdisk_bcs::init_sdk(const char *ak, const char *sk, const char *host)
{
	return new bcs_sdk(ak, sk, host);
}

bcs_sdk *
netdisk_bcs::init_sdk(std::string ak, std::string sk, std::string host)
{
	return new bcs_sdk(ak.c_str(), sk.c_str(), host.c_str());
}

/*
 * Do some REAL works.
 */

bool
netdisk_bcs::is_exist(const char *file)
{
	if ( access(file, F_OK) == 0 ) {
		return true;
	} else {
		return false;
	}
}

bool
netdisk_bcs::is_exist(std::string file)
{
	return this->is_exist(file.c_str());
}

int
netdisk_bcs::upload(const char *file, const char *remote_file)
{
	if ( status() == -1 ) return -1;

	int ret;
	if ( is_exist(file) ) {
		response resp;
		std::cout<<"get_handler:"<<get_handler()<<endl
			<<"get_bucket:"<<m_bcs_bucket<<endl;

		ret = get_handler()->put_object(
				get_bucket().c_str(), remote_file, file, resp);
		return ret;
	} else {
		return -1;
	}
}

int
netdisk_bcs::upload(std::string file, std::string remote_file)
{
	return this->upload(file.c_str(), remote_file.c_str());
}

int
netdisk_bcs::download(const char *file, const char *remote_file)
{
	if ( status() == -1 ) return -1;
	int ret;

	response resp;
	ret = get_handler()->get_object(\
			get_bucket().c_str(), remote_file, file, resp);
	return ret;
}

int
netdisk_bcs::download(std::string file, std::string remote_file)
{
	return this->download(file.c_str(), remote_file.c_str());
}

int
netdisk_bcs::remove(const char *remote_file)
{
	if ( status() == -1 ) return -1;

	int ret;
	response resp;
	ret = get_handler()->delete_object(get_bucket().c_str(), remote_file, resp);

	return ret;
}

int
netdisk_bcs::remove(std::string remote_file)
{
	return this->remove(remote_file.c_str());
}

std::string
netdisk_bcs::list(const char *bkt)
{
	if ( status() == -1 ) return string();

	int ret;
	response resp;
	ret = get_handler()->list(bkt, resp);

	if ( ret == 0 ) {
		return resp.body;
	} else {
		return string();
	}
//	std::cout<<resp.body<<endl;
}

std::string
netdisk_bcs::list(std::string bkt)
{
	return this->list(bkt.c_str());
}

int
netdisk_bcs::dir_upload(const char *dir)
{}

int
netdisk_bcs::dir_upload(std::string dir)
{}



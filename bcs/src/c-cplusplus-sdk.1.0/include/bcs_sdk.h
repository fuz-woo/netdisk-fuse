/***************************************************************************                                                                       
 *                            
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * 
***************************************************************************/

#include <stdio.h>
#include <string>
#include "curl/curl.h"
#include "openssl/evp.h"        
#include "openssl/hmac.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include <vector>
#include <sys/stat.h> 
#include <unistd.h>  
#include <map>
#include <dirent.h>
using namespace std;


#ifndef BCS_SDK_H_INCLUDED 
#define BCS_SDK_H_INCLUDED 



static const int BCS_BUFF_AUTH_SIZE = 128;
static const int BCS_BUFF_SIGN = 4096;
static const char * BCS_FLAG_METHOD = "Method"; 
static const char * BCS_FLAG_BUCKET = "Bucket"; 
static const char * BCS_FLAG_OBJECT = "Object"; 
static const char * BCS_FLAG_IP = "Ip";
static const char * BCS_FLAG_TIME = "Time";
static const char * BCS_FLAG_SIZE = "Size";
static const char * BCS_FLAG_HTTPS = "https";
static const char * BCS_FLAG_AK = "ak";
static const char * BCS_FLAG_SK = "sk";
static const char * BCS_FLAG_HOST = "host";
static const char * BCS_FLAG_VERSIONS = "versions";
static const char * BCS_FLAG_VERSIONING = "versioning";
static const char * BCS_FLAG_SUPERFILE_PART_SUFFIX = "superfile_part_suffix";
static const char * BCS_FLAG_QUERYSTRING = "querystring_opt";



/*
 * ��¼ÿ������Ľ��
 */
struct response
{
    map<string, string> header;//��¼bcs���󷵻ص�Header����key/value��ʽ�洢
    string body; //��¼bcs���ص�body����һЩ������Ϣ
    int status_code;//��¼bcs���ص�״̬��
};

/*
 * map<string, string> *opt
 * opt ��ȡֵ����Ϊ
 * opt["https"] = "1" //ȡֵΪ1��ʾʹ��https, �����ʾʹ��http
 * opt["Time"] ��ʾ����ʱ��
 * opt["Size"] ��ʾ�����ļ���С
 * opt["Ip"] ��ʾ����ip
 * opt["versioning"] = "1" ��ʾҪʹ�ð汾
 * opt["ak"] ��ʾ�������ʹ�ø�ak
 * opt["sk"] ��ʾ�������ʹ�ø�sk
 * opt["host"] ��ʾ�������ʹ�ø�host
 * opt["start"] ��ʾList objectʱ��Ŀ�ʼλ��
 * opt["limit"] ��ʾList Objectʱ���object��������
 * opt["superfile_part_suffix"] ��ʾ�ϴ�superfile���ļ��ĺ�׺�������õĻ�Ĭ��ֵΪ_part_
 * opt["versions"] = "1" ��ʾ��ȡָ��bucket��object����ʷ�汾
 * opt["querystring_opt"] ��ʾ����������url������Щquerystring_opt
 *      ����opt["querystring_opt"]="response-cache-control=private&x-bs-version=xxxxxxxx" 
 */


class bcs_sdk
{
    public:
        /* --------------------------------------------------------------------------*/
        /**
         * @brief bcs_sdk�Ĺ��캯��
         *
         * @Param ak ȫ�ֵ�ak��ĳ�β�������ʹ��opt�޸���β�����ak
         * @Param sk ȫ�ֵ�sk��ĳ�β�������ʹ��opt�޸���β�����sk
         * @Param hostname ȫ�ֵ�hostname��ĳ�β�������ʹ��opt�޸���β�����hostname
         * @Param log_file_path ��¼��־��·��
         */
        /* ----------------------------------------------------------------------------*/
        bcs_sdk(const char *ak, const char *sk, const char *hostname, const char *log_file_path = "log/bcs.log");
        
        /* --------------------------------------------------------------------------*/
        /**
         * @brief bcs_sdk�Ĺ��캯��
         *
         * @Param ak ȫ�ֵ�ak��ĳ�β�������ʹ��opt�޸���β�����ak
         * @Param sk ȫ�ֵ�sk��ĳ�β�������ʹ��opt�޸���β�����sk
         * @Param hostname ȫ�ֵ�hostname��ĳ�β�������ʹ��opt�޸���β�����hostname
         * @Param fd ��¼��־��Ϣ�ļ���fd���������Ҫ��־���ɽ��ò�������Ϊstderr
         *           ���� FILE * fd = fopen("/dev/null", "w"),Ȼ�󽫸�fd����
         */
        /* ----------------------------------------------------------------------------*/
        bcs_sdk(const char *ak, const char *sk, const char *hostname, FILE *fd);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief create_bucket ����bucket
         *
         * @Param bucket ����bucket������
         * @Param res ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int create_bucket(const char *bucket, response &res, map<string, string> *header = NULL,
                map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief delete_bucket ɾ��bucket
         *
         * @Param bucket ɾ��Bucket������
         * @Param res ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         *
         */
        /* ----------------------------------------------------------------------------*/
        int delete_bucket(const char *bucket, response &res, map<string, string> *header = NULL, 
                map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_url ���ɸ��ֲ�����url
         *
         * @Param url ���ɺ��url
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param method �����ķ���,���� PUT GET HEAD DELETE
         * @Param opt ��ѡ����
         */
        /* ----------------------------------------------------------------------------*/
        void generate_url(string& url, string bucket, string object, 
                string method, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_object �ϴ��ļ� ,����fileָ���ϴ��ļ��ı���·��
         *
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param file �ϴ��ļ��ı���·��
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int put_object(const char *bucket, const char *object, const char *file, 
                response &res, map<string, string> *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_object_from_memory �ϴ��ļ��������ļ������Ǵ����ڲ���content���
         *
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param content �洢�ϴ��ļ�������,content.length()��¼���ݵĳ���
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int put_object_from_memory(const char *bucket, const char *object, string content, 
                response &res, map<string, string> *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_superfile �ϴ������ļ� 
         * �ϴ����ļ��ȱ��ֳ����ɸ�256M�����ļ��ϴ���Ȼ���ٺϲ���Щ���ļ������һ��superfile
         *
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param file �ϴ��ļ��ı���·��
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int put_superfile(const char *bucket, const char *object, const char* file, 
                response &res, map<string, string>  *header = NULL,  map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief merge �ϲ��Ѿ��ϴ������ɸ��ļ������һ��superfile�ļ� 
         *
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param object_url_list ��¼�Ѿ��ϴ��ļ���url����ʽΪbs://bucket/object
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int merge(const char *bucket, const char *object, vector<string>  object_url_list, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief copy_object copy object����
         *
         * @Param bucket Ŀ��bucket
         * @Param object Ŀ��object
         * @Param source_url Դ��url,���� bs://bucket/object
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int copy_object(const char *bucket, const char *object,  const char *source_url,  
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief get_object �����ļ�
         *
         * @Param bucket ������bucket
         * @Param object ������Object
         * @Param file   ���غ���ļ�·��
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int get_object(const char *bucket, const char *object, const char *file, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief get_object_to_memory
         * �����ļ��������غ�����ݴ������ڴ��У����洢�ڱ���res.body��
         *
         * @Param bucket ������bucket
         * @Param object ������Object
         * @Param res  ��¼����Ľ��,������سɹ���res.body��洢���غ���ļ�
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int get_object_to_memory(const char *bucket, const char *object, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief head_object head object ,��ȡ�ļ�������
         *
         * @Param bucket ������bucket
         * @Param object ������Object
         * @Param res  ��¼����Ľ��
         *        res.body���¼ԭʼ��head������,res.header���¼������Ľ������k/v��ʽ�洢
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int head_object(const char *bucket, const char *object, response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief delete_object ɾ���ļ�
         *
         * @Param bucket ������bucket
         * @Param object ������Object
         * @Param res  ��¼����Ľ��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int delete_object(const char *bucket, const char *object,  response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief list list bucket ���� list object 
         *
         * @Param bucket ��bucketΪ�յ�ʱ��Ϊlist bucket���ܣ���bucket��Ϊ�յ�ʱ��list��bucket�µ�object
         * @Param res ��¼����Ľ�����������ɹ���res.body���¼bucket or object���б�Ϊһ��json��
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int list(const char *bucket, response &res, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief list_version list ��ȡָ��bucket��object����ʷ�汾
         *
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param res   ��¼���صĽ��������ɹ���res.body��Ϊ��ʷ�汾���б�Ϊһ��json��
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int list_version(const char *bucket, const char *object, response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief dir_upload �����ϴ����ϴ�ָ��Ŀ¼�µ������ļ�
         *
         * @Param bucket ������bucket 
         * @Param prefix �ϴ�����ļ�����ǰ׺
         * @Param local_dir �ƶ�Ŀ¼
         * @Param failed_file ��¼ʧ�ܺ���ļ���
         * @Param header �����header,��ѡ����
         * @Param opt ��ѡ����
         *
         * @Returns  1 : ��ʾʧ�ܣ����ڸ��Բ�������֮���
         *           0 ����ʾ�ɹ���ʧ�ܵ��ļ�������Ҫ�鿴failed_file.size()
         */
        /* ----------------------------------------------------------------------------*/
        int dir_upload(const char *bucket, const char *prefix, const char *local_dir, 
               vector<string>  &failed_file, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        ~bcs_sdk();
    private:
        /* --------------------------------------------------------------------------
         * ak sk ��Ҫ��ƽ̨������ ,hostnameΪ������ĵ�ַ  
         ----------------------------------------------------------------------------*/
        string ak;
        string sk;
        string hostname;
        /* ----------------------------------------------------------------------------*/
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_json_object_list 
         * ����superfile��json��
         * @Param object_list ��¼���ļ���url�б���ʽΪbs://bucket/object
         * @Param json_object_list ���ɵ�json��
         * @Param res ��¼����״̬
         *
         * @Returns  1 : ��ʾʧ��
         *           0 ����ʾ�ɹ�
         */
        /* ----------------------------------------------------------------------------*/
        int generate_json_object_list(vector<string>  object_list, string& json_object_list, response &res);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief recursion_dir 
         * �ݹ��ϴ��ļ�
         * @Param bucket ������Bucket 
         * @Param local_dir ���ص�Ŀ¼��
         * @Param prefix  �ϴ�����ļ�ǰ׺
         * @Param failed_file ��¼�ϴ�ʧ�ܵ��ļ���
         * @Param header ����header����ѡ��
         * @Param opt  ��ѡ��
         */
        /* ----------------------------------------------------------------------------*/
        void recursion_dir(const char *bucket, const char *local_dir, const char *prefix,
                vector<string>  &failed_file, map<string, string>  *header = NULL, map<string, string> *opt = NULL);  
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_sign 
         * ����ǩ��
         * @Param sign_buff ��¼������ɵ�ǩ��
         * @Param sign_buff_size ǩ�����ȵ�����
         * @Param ak ����ǩ���õ���ak
         * @Param sk ����ǩ���õ���sk
         * @Param sign_flag ����ǩ���õ��ı�ʶ��Ĭ����MBO
         * @Param method ����������PUT GET DELETE HEAD֮���
         * @Param bucket ������bucket
         * @Param object ������object
         * @Param ip     ip���ƣ���ѡ��
         * @Param sign_time ʱ�����ƣ���ѡ��
         * @Param sign_size �ļ���С�����ƣ���ѡ��
         */
        /* ----------------------------------------------------------------------------*/
        void generate_sign(char *sign_buff, int sign_buff_size, string ak, string sk, string sign_flag,
                string method, string bucket, string object, string ip, int sign_time, int sign_size); 
};

#endif // BCS_SDK_H_INCLUDED

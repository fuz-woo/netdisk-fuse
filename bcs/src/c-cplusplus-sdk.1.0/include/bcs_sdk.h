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
 * 记录每次请求的结果
 */
struct response
{
    map<string, string> header;//记录bcs请求返回的Header，以key/value形式存储
    string body; //记录bcs返回的body或者一些错误信息
    int status_code;//记录bcs返回的状态码
};

/*
 * map<string, string> *opt
 * opt 的取值可以为
 * opt["https"] = "1" //取值为1表示使用https, 否则表示使用http
 * opt["Time"] 表示限制时间
 * opt["Size"] 表示限制文件大小
 * opt["Ip"] 表示限制ip
 * opt["versioning"] = "1" 表示要使用版本
 * opt["ak"] 表示这次请求使用该ak
 * opt["sk"] 表示这次请求使用该sk
 * opt["host"] 表示这次请求使用该host
 * opt["start"] 表示List object时候的开始位置
 * opt["limit"] 表示List Object时候的object个数限制
 * opt["superfile_part_suffix"] 表示上传superfile子文件的后缀，不设置的话默认值为_part_
 * opt["versions"] = "1" 表示获取指定bucket、object的历史版本
 * opt["querystring_opt"] 表示这次请求里的url附带这些querystring_opt
 *      例如opt["querystring_opt"]="response-cache-control=private&x-bs-version=xxxxxxxx" 
 */


class bcs_sdk
{
    public:
        /* --------------------------------------------------------------------------*/
        /**
         * @brief bcs_sdk的构造函数
         *
         * @Param ak 全局的ak，某次操作可以使用opt修改这次操作的ak
         * @Param sk 全局的sk，某次操作可以使用opt修改这次操作的sk
         * @Param hostname 全局的hostname，某次操作可以使用opt修改这次操作的hostname
         * @Param log_file_path 记录日志的路径
         */
        /* ----------------------------------------------------------------------------*/
        bcs_sdk(const char *ak, const char *sk, const char *hostname, const char *log_file_path = "log/bcs.log");
        
        /* --------------------------------------------------------------------------*/
        /**
         * @brief bcs_sdk的构造函数
         *
         * @Param ak 全局的ak，某次操作可以使用opt修改这次操作的ak
         * @Param sk 全局的sk，某次操作可以使用opt修改这次操作的sk
         * @Param hostname 全局的hostname，某次操作可以使用opt修改这次操作的hostname
         * @Param fd 记录日志信息文件的fd，如果不需要日志，可将该参数设置为stderr
         *           或者 FILE * fd = fopen("/dev/null", "w"),然后将该fd传入
         */
        /* ----------------------------------------------------------------------------*/
        bcs_sdk(const char *ak, const char *sk, const char *hostname, FILE *fd);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief create_bucket 创建bucket
         *
         * @Param bucket 创建bucket的名字
         * @Param res 记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int create_bucket(const char *bucket, response &res, map<string, string> *header = NULL,
                map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief delete_bucket 删除bucket
         *
         * @Param bucket 删除Bucket的名字
         * @Param res 记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         *
         */
        /* ----------------------------------------------------------------------------*/
        int delete_bucket(const char *bucket, response &res, map<string, string> *header = NULL, 
                map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_url 生成各种操作的url
         *
         * @Param url 生成后的url
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param method 操作的方法,例如 PUT GET HEAD DELETE
         * @Param opt 可选参数
         */
        /* ----------------------------------------------------------------------------*/
        void generate_url(string& url, string bucket, string object, 
                string method, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_object 上传文件 ,参数file指向上传文件的本地路径
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param file 上传文件的本地路径
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int put_object(const char *bucket, const char *object, const char *file, 
                response &res, map<string, string> *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_object_from_memory 上传文件，但是文件内容是存在于参数content里的
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param content 存储上传文件的内容,content.length()记录内容的长度
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int put_object_from_memory(const char *bucket, const char *object, string content, 
                response &res, map<string, string> *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief put_superfile 上传超大文件 
         * 上传的文件先被分成若干个256M的子文件上传，然后再合并这些子文件，组成一个superfile
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param file 上传文件的本地路径
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int put_superfile(const char *bucket, const char *object, const char* file, 
                response &res, map<string, string>  *header = NULL,  map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief merge 合并已经上传的若干个文件，组成一个superfile文件 
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param object_url_list 记录已经上传文件的url，格式为bs://bucket/object
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int merge(const char *bucket, const char *object, vector<string>  object_url_list, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief copy_object copy object操作
         *
         * @Param bucket 目的bucket
         * @Param object 目的object
         * @Param source_url 源的url,例如 bs://bucket/object
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int copy_object(const char *bucket, const char *object,  const char *source_url,  
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief get_object 下载文件
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的Object
         * @Param file   下载后的文件路径
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int get_object(const char *bucket, const char *object, const char *file, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief get_object_to_memory
         * 下载文件，将下载后的内容储存于内存中，即存储于变量res.body里
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的Object
         * @Param res  记录请求的结果,如果下载成功，res.body里存储下载后的文件
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int get_object_to_memory(const char *bucket, const char *object, 
                response &res, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief head_object head object ,获取文件的属性
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的Object
         * @Param res  记录请求的结果
         *        res.body里记录原始的head请求结果,res.header里记录解析后的结果，以k/v形式存储
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int head_object(const char *bucket, const char *object, response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief delete_object 删除文件
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的Object
         * @Param res  记录请求的结果
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int delete_object(const char *bucket, const char *object,  response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief list list bucket 或者 list object 
         *
         * @Param bucket 当bucket为空的时候，为list bucket功能，当bucket不为空的时候，list该bucket下的object
         * @Param res 记录请求的结果，如果请求成功，res.body里记录bucket or object的列表，为一个json串
         * @Param opt 可选常数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int list(const char *bucket, response &res, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief list_version list 获取指定bucket、object的历史版本
         *
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param res   记录返回的结果，如果成功，res.body里为历史版本的列表，为一个json串
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int list_version(const char *bucket, const char *object, response &res,
                map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief dir_upload 批量上传，上传指定目录下的所有文件
         *
         * @Param bucket 操作的bucket 
         * @Param prefix 上传后的文件名的前缀
         * @Param local_dir 制定目录
         * @Param failed_file 记录失败后的文件名
         * @Param header 请求的header,可选参数
         * @Param opt 可选参数
         *
         * @Returns  1 : 表示失败，由于各自参数错误之类的
         *           0 ：表示成功，失败的文件个数需要查看failed_file.size()
         */
        /* ----------------------------------------------------------------------------*/
        int dir_upload(const char *bucket, const char *prefix, const char *local_dir, 
               vector<string>  &failed_file, map<string, string>  *header = NULL, map<string, string> *opt = NULL);
        ~bcs_sdk();
    private:
        /* --------------------------------------------------------------------------
         * ak sk 需要到平台上申请 ,hostname为发请求的地址  
         ----------------------------------------------------------------------------*/
        string ak;
        string sk;
        string hostname;
        /* ----------------------------------------------------------------------------*/
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_json_object_list 
         * 生成superfile的json串
         * @Param object_list 记录子文件的url列表，形式为bs://bucket/object
         * @Param json_object_list 生成的json串
         * @Param res 记录返回状态
         *
         * @Returns  1 : 表示失败
         *           0 ：表示成功
         */
        /* ----------------------------------------------------------------------------*/
        int generate_json_object_list(vector<string>  object_list, string& json_object_list, response &res);
        /* --------------------------------------------------------------------------*/
        /**
         * @brief recursion_dir 
         * 递归上传文件
         * @Param bucket 操作的Bucket 
         * @Param local_dir 本地的目录名
         * @Param prefix  上传后的文件前缀
         * @Param failed_file 记录上传失败的文件名
         * @Param header 设置header，可选项
         * @Param opt  可选项
         */
        /* ----------------------------------------------------------------------------*/
        void recursion_dir(const char *bucket, const char *local_dir, const char *prefix,
                vector<string>  &failed_file, map<string, string>  *header = NULL, map<string, string> *opt = NULL);  
        /* --------------------------------------------------------------------------*/
        /**
         * @brief generate_sign 
         * 生成签名
         * @Param sign_buff 记录最后生成的签名
         * @Param sign_buff_size 签名长度的限制
         * @Param ak 生成签名用到的ak
         * @Param sk 生成签名用到的sk
         * @Param sign_flag 生成签名用到的标识，默认是MBO
         * @Param method 方法，例如PUT GET DELETE HEAD之类的
         * @Param bucket 操作的bucket
         * @Param object 操作的object
         * @Param ip     ip限制，可选项
         * @Param sign_time 时间限制，可选项
         * @Param sign_size 文件大小的限制，可选项
         */
        /* ----------------------------------------------------------------------------*/
        void generate_sign(char *sign_buff, int sign_buff_size, string ak, string sk, string sign_flag,
                string method, string bucket, string object, string ip, int sign_time, int sign_size); 
};

#endif // BCS_SDK_H_INCLUDED

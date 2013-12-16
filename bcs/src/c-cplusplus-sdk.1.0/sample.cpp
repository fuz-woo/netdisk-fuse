/***************************************************************************       
 *                            
 * Copyright (c) 2012 Baidu.com, Inc. All Rights Reserved
 * 
***************************************************************************/


#include "include/bcs_sdk.h"

void print_header(map<string, string> mp)
{
   map<string, string>::iterator it = mp.begin();
   for(; it != mp.end(); ++it)
   {
	printf("key:[%s], value:[%s]\n", (it->first).c_str(), (it->second).c_str());
   }
}

int main()
{
    map<string, string> header;
    map<string, string> opt;
    response res;
    int ret;
    // ak ,sk �뵽http://yun.baidu.com���룬������bucket�����ö��
    char *ak = "FFFFFFFFFFFFFFFFFF";//���ﻻ�����ak
    char *sk = "EEEEEEEEEEEEEEEEEE";//���ﻻ�����sk
    char bucket[130] = "your-bucket-name";//���ﻻ�����Bucket����
    char *host = "bcs.duapp.com";
    //����,���ĸ����������ƶ�log�Ĵ��λ�ã�Ĭ��Ϊlog/bcs.log,����ȷ��logĿ¼����
    bcs_sdk *bcs = new bcs_sdk(ak, sk, host);

    char *delete_bucket_name = "private-d";
    printf("bucket is:[%s]\n", bucket);

    ret = bcs->put_object(bucket, "put_object_sample2", "upload.txt", res);
    puts("");
    printf("put object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
       
    //put object ,�ϴ��ļ�������header
    header.clear();
    header["x-bs-meta-map"] = "test";
  
    ret = bcs->put_object(bucket, "put_object_sample1", "upload.txt", res, &header);
    puts("");
    printf("put object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
   

    //put object ,�ϴ��ļ���������header
    ret = bcs->put_object(bucket, "put_object_sample2", "upload.txt", res);
    puts("");
    printf("put object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  

    //put object�������ļ������ݴ洢���ڴ��еı������������content
    string content = "test";
    ret = bcs->put_object_from_memory(bucket, "put_object_from_memory.txt", content, res);
    puts("");
    printf("put object from memory ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
    
    
    //put object�������ļ������ݴ洢���ڴ��еı������������content
    char content_with_binary[] = {'a', 'b', '\0', 'c', 'd', '\0'};
    content = string(content_with_binary, 6);
    ret = bcs->put_object_from_memory(bucket, "put_object_from_memory_3.txt", content, res);
    puts("");
    printf("put object from memory which is binary ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
   
    //get object ,�����غ���ļ�д���ڴ��� 
    ret = bcs->get_object_to_memory(bucket, "put_object_from_memory.txt", res);
    puts("");
    printf("get object to memory ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    if(ret)
    {
        printf("get object error, error msg:[%s]\n", res.body.c_str());
    }
    else
    {
    	printf("get object success, object length is:[%u] , object content is:[%s]\n",
		       	res.body.length(), res.body.c_str());
    }
    puts(""); 

    //get object, �����ļ� 
    opt.clear();
    opt[BCS_FLAG_QUERYSTRING] = "response-cache-control=private";
    ret = bcs->get_object(bucket, "put_object_sample1", "download.txt", res, NULL, &opt);
    puts("");
    printf("get object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    if(ret)
    {
        printf("get object error, error msg:[%s]\n", res.body.c_str());
    }
    puts("");  

    //head object, ��ȡobject������
    ret = bcs->head_object(bucket, "put_object_sample1", res);
    puts("");
    printf("head object ret:[%d]\n", ret);
    print_header(res.header);
    printf("status_code:[%d]\n", res.status_code);
    puts("");  
    
    //copy object , ����object
    string src_url = "bs://";
    src_url.append(bucket);
    src_url.append("/put_object_sample1");
    ret = bcs->copy_object(bucket, "copy_object", src_url.c_str(), res);
    puts("");
    printf("copy object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  

    //copy object , copy �Լ����޸�meta����
    header.clear();
    header["x-bs-meta-debug"] = "copy test"; 
    src_url = "bs://";
    src_url.append(bucket);
    src_url.append("/put_object_sample1");
    ret = bcs->copy_object(bucket, "put_object_sample1", src_url.c_str(), res, &header);
    puts("");
    printf("copy object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");

    //list bucket,��ʾ���е�Bucket
     
    ret = bcs->list("", res);
    puts("");
    printf("list bucket ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    if(ret == 0)
    {
        printf("list bucket, bucket info:[%s]\n", res.body.c_str());
    }
    else
    {
        printf("list bucket failed, error msg:[%s]\n", res.body.c_str());
    }

    //list object, ��ʾָ��bucket��object
    opt.clear();
    opt["start"] = "3";
    opt["limit"] = "3";
    ret = bcs->list(bucket, res, &opt);
    puts("");
    printf("list object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    if(ret == 0)
    {
       printf("list object, object info:[%s]\n", res.body.c_str());
    }
    else
    {
       printf("list object failed, error msg:[%s]\n", res.body.c_str());
    }

    //֧�ְ汾�ϴ�object
    opt.clear();
    opt["versioning"] = "1";
    ret = bcs->put_object(bucket, "copy_objcet", "upload.txt", res, NULL, &opt);
    puts("");
    printf("put object for versioning ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts(""); 
    

    //list ָ��bucket object����ʷ�汾, �������� 
    ret = bcs->list_version(bucket, "copy_object", res);
    puts("");
    printf("list version ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    if(ret == 0)
    {
        printf("list version, version info:[%s]\n", res.body.c_str());
    }
    else
    {
       printf("list version failed, error msg:[%s]\n", res.body.c_str());
    }

    //delete object , ɾ��object
    ret = bcs->delete_object(bucket, "copy_object", res);
    puts("");
    printf("delete object ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
    
    //delete object , ɾ��һ�������ڵ�object
    ret = bcs->delete_object(bucket, "object_not_exist", res);
    puts("");
    printf("delete object which not exist, ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts(""); 

    //upload_dir �����ϴ����ϴ�ָ��Ŀ¼�����е�object 
    vector<string> failed_file;
    ret = bcs->dir_upload(bucket, "prefix/", "lib", failed_file);
    puts("");
    printf("dir upload ret:[%d]\n", ret);
    if(ret == 0 && failed_file.size() == 0)
    {
        printf("upload all file success\n");
    }
    else
    {
        printf("upload file failed\n");
    }
    for(int i = 0 ; i < failed_file.size(); ++i)
    {
        printf("failed_file_%d:[%s]\n", i, failed_file[i].c_str());
    }
    puts(""); 
    
    //create bucket ,����bucket
    ret = bcs->create_bucket(delete_bucket_name, res);
    puts("");
    printf("create bucket ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
    //delete bucket ,ɾ��bucket
    ret = bcs->delete_bucket(delete_bucket_name, res);
    puts("");
    printf("delete bucket ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
  
    //put superfile ,�ϴ������ļ�
    ret = bcs->put_superfile(bucket, "sp1", "sample.cpp", res);
    puts("");
    printf("put superfile ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  

    //merge,�ϲ��ƴ洢�ϵ�object
    vector<string> object_url_list;
    object_url_list.push_back(src_url);
    ret = bcs->merge(bucket, "merge", object_url_list, res);
    puts("");
    printf("merge ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
    delete bcs;
   
    FILE *fd = fopen("log/log", "a+");
    bcs_sdk *bcs2 = new bcs_sdk(ak, sk, host, fd);
    ret = bcs2->put_superfile(bucket, "sp2", "sample.cpp", res);
    puts("");
    printf("put superfile ret:[%d]\n", ret);
    printf("status_code:[%d]\n", res.status_code);
    print_header(res.header);
    printf("respond body:[%s]\n", res.body.c_str());
    puts("");  
    delete bcs2;
    fclose(fd);
    return 0;
}

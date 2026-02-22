
/*
#define STA_BYTES_IMPLEMENTATION
#include "sta-master/sta/sta_bytes.h"
*/


//void bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len);
//void bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len);

void * a_bytes_copy_malloc(const void * bytes,int bytes_size);

void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2);

void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3);



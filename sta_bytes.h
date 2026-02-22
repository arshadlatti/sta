
#ifndef STA_BYTES_H
#define STA_BYTES_H

//#include "STA/sta_def.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
#define STA_BYTES_IMPLEMENTATION
#include "sta-master/sta/sta_bytes.h"
*/


//void bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len);
//void bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len);

void * a_bytes_copy_malloc(const void * bytes,int bytes_size);

void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2);

void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3);




#ifdef __cplusplus
}
#endif

#endif


#ifdef STA_BYTES_IMPLEMENTATION

#ifndef STA_BYTES_IMPLEMENTATION_INCLUDED
#define STA_BYTES_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif



void * a_bytes_copy_malloc(const void * bytes,int bytes_size)
{
	if(!bytes_size) return 0;
	void * r = malloc(bytes_size);
	if(r)
	 memcpy(r,bytes,bytes_size);
return r;
}


void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2)
{
	if(!bytes_size_1 && !bytes_size_2) return 0;
	char * r = (char*)malloc(bytes_size_1 + bytes_size_2);
	if(r)
	{
		if(bytes_size_1)
			memcpy(r,bytes_1,bytes_size_1);
		if(bytes_size_2)
			memcpy(&r[bytes_size_1],bytes_2,bytes_size_2);
	}
	return r;
}


void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3)
{
		if(!bytes_size_1 && !bytes_size_2) return 0;
	char * r = (char*)malloc(bytes_size_1 + bytes_size_2 + bytes_size_3);
	if(r)
	{
		if(bytes_size_1)
			memcpy(r,bytes_1,bytes_size_1);
		if(bytes_size_2)
			memcpy(&r[bytes_size_1],bytes_2,bytes_size_2);
		if(bytes_size_3)
			memcpy(&r[bytes_size_1 + bytes_size_2],bytes_3,bytes_size_3);
	}
	return r;
}

#ifdef __cplusplus
}
#endif

#endif

#endif

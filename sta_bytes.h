
#ifdef STA_PUBLIC_API


/*
#define STA_BYTES_IMPLEMENTATION
#include "sta-main/sta_bytes.h"
*/


void a_bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len);
void a_bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len);

void * a_bytes_copy_malloc(const void * bytes,int bytes_size);

void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2);

void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3);

 void * a_bytes_can_null_term_from_file_malloc(const char * file_name,a_bool be_null_term_output,long * out_size);
 
a_bool a_bytes_to_file(const char * file_name,const void * buffer,long buffer_size);
void * a_bytes_from_file_malloc(const char * file_name,long * out_size);
 void * a_bytes_from_file_part_malloc(const char * file_name,int offset,int size);
 
 void * a_bytes_pointer_at_offset(void * base,int offset);
void * a_bytes_malloc(void ** out_ptr_ptr,size_t size);
//void a_free(void * p);
#define a_free(p) {if(p) free(p);}
#define a_malloc(var,size) a_bytes_malloc((void**)&var,size)


 // void * a_bytes_from_file_header_malloc(const char * file_name,int size);
#define a_bytes_from_file_header_malloc(file_name,size) a_bytes_from_file_part_malloc(file_name,0,size)

char * a_string_from_file_malloc(const char * file_name);
a_bool a_string_to_file(const char * file_name,const char * text);

//
#define a_array_to_file(file_name,array_name,array_size,name_of_array_type) a_bytes_to_file(file_name,array_name,array_size * sizeof(name_of_array_type))


// var uint8_t * data; long data_size;
 #define a_data_bytes_from_file_malloc(filename) long data_size; uint8_t * data = a_bytes_from_file_malloc(filename,&data_size);

 
#define a_data_bytes_from_file_part_malloc(filename,offset,size) long data_size; uint8_t * data = a_bytes_from_file_part(filename,offset,size);



#define a_data_bytes_to_file(filename) a_bytes_to_file(filename,data,data_size);


// A Project By Arshad Latti
 

#endif

#ifndef STA_DEF_H_
#define STA_DEF_H_

#ifndef STA_NO_STDINT_H
#include <stdint.h>
#endif

/* 
//#define STA_NO_STDINT_H
//#define STA_COMPILER_VC_6

#ifdef STA_COMPILER_VC_6
#include "stdint-vc6.h"
#endif 
*/
 
#define a_bool int 
#define a_true 1
#define a_false 0
#define a_null 0


// PACKED 
/*
A_PACKED_BEGIN

struct disk_data
{
char x;
int y;
}A_PACKED;

A_PACKED_END
*/

#ifndef STA_PACKED_OPTIONS
//auto detect compiler
#define STA_PACKED_OPTIONS
#define STA_PACKED_COMPILER_GCC
//#define STA_PACKED_COMPILER_MSVC
//#define STA_PACKED_COMPILER_TCC

#endif

#ifdef STA_PACKED_COMPILER_GCC
#define A_PACKED_BEGIN
#define A_PACKED_END
#define A_PACKED __attribute__((packed))
#endif

#ifdef STA_PACKED_COMPILER_MSVC
#define A_PACKED_BEGIN #pragma pack( push,1 )
#define A_PACKED_END #pragma pack( pop )
#define A_PACKED 
#endif

// Framebuffer Image
#define framebuffer_pixel_t uint32_t

struct s_framebuffer_image
{
	framebuffer_pixel_t * pixels;
	int32_t h,w,pitch;//all in pixels
};
typedef struct s_framebuffer_image framebuffer_image_t;


//AV_PIX_FMT_BGRA
struct s_framebuffer_color
{
uint8_t b;
uint8_t g;
uint8_t r;
uint8_t a;
};
typedef struct s_framebuffer_color framebuffer_color_t;

//
struct s_clip_rect
{
	int32_t x,y,h,w;
};
typedef struct s_clip_rect clip_rect_t;

struct s_a_rect
{
	int32_t x,y,x2,y2;
};
typedef struct s_a_rect a_rect_t;


struct s_a_rect_size
{
	int h,w;
};
typedef struct s_a_rect_size a_rect_size_t;



struct s_a_point
{
	int x,y;
};
typedef struct s_a_point a_point_t;



typedef struct s_a_date_time a_date_time_t;
struct s_a_date_time
{
	uint16_t year;
	uint8_t month;
	uint8_t day;//date
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t flags;// bit 1.0=utc 2.0=24hr 3.0=date valid 4.0=time valid 5.0valid 6-8=resvered FF=not valid
};

//

#define A_MIN(a,b) ((a) < (b) ? (a) : (b))
#define A_MAX(a,b) ((a) < (b) ? (b) : (a))
#define A_KB(k) ((k) * 1024)
#define A_MB(mb) (A_KB((mb) * 1024))
#define A_GB(gb) (A_MB((gb) * 1024))
#define A_TB(tb) (A_GB((tb) * 1024))


//if.not if.index
#define if_not(expr) if(!(expr))
#define if_index(expr) if((expr) >= 0)



#define for_i(i_max) int i; for(i=0;i<i_max;i++)
#define for_i_(i_max) for(i=0;i<i_max;i++)



#define for_var(i,i_max) int i; for(i=0;i<i_max;i++)
#define for_var_(i,i_max) for(i=0;i<i_max;i++)
// for_j
#define for_j(x_max) for_var(j,x_max)
#define for_j_(x_max) for_var_(j,x_max)
		
// for_k
#define for_k(x_max) for_var(k,x_max)
#define for_k_(x_max) for_var_(k,x_max)
	
	
// for_x for_y
#define for_x(x_max) int x; for(x=0;x<x_max;x++)
#define for_x_(x_max) for(x=0;x<x_max;x++)
	
#define for_y(y_max) int y; for(y=0;y<y_max;y++)
#define for_y_(y_max) for(y=0;y<y_max;y++)
	
// #define for_i_xy(w,h)	int i_x,i_y,i_xy,i_xy_max; i_xy_max = w*h; for(i_xy=0;i_xy < i_xy_max;i_xy++

// use }} 
// #define for_xy_next }}
#define next_xy }}
#define for_xy(w,h)	int i_x,i_y;\
for(i_y = 0; i_y < h ; i_y++){\
for(i_x = 0; i_x < w ; i_x++){

#define for_xy_(w,h) for(i_y = 0; i_y < h ; i_y++){\
for(i_x = 0; i_x < w ; i_x++){

#define for_fi(fi) for_xy(fi->w,fi->h)
#define for_fi_(fi) for_xy_(fi->w,fi->h)
#define next_fi }


#define for_i_cc(s) int cc = strlen(s); int i; for(i=0;i<cc;i++)
#define for_i_cc_(s) cc = strlen(s); for(i=0;i<cc;i++)	
//#define for_cc(s) for_i_cc(s)


#define return_if_null(expr) if(!(expr)) return 0
#define return_if_zero(expr) if(!(expr)) return 0
#define return_if_fail(expr) if((expr) != 0) return 0
#define return_if_not_find(expr) if((expr) < 0) return 0

// ptr
//#define if
#define if_null(expr) if(!(expr))
//status	
#define if_success(expr) if((expr) == 0)
#define if_fail(expr) if((expr) != 0)
//index
#define if_not_find(expr) if((expr) < 0)
#define if_find(expr) if((expr) >=0)
	



#define puts_i(i) printf("%i\n",i);
#define puts_s_i(s,i) printf("%s %i\n",s,i);


#define IFF(expr,on_true,on_false) ((expr)? on_true : on_false)


#define bitwise_and(a,b) ((a)&(b))
#define bitwise_or(a,b) ((a)|(b))
#define bitwise_xor(a,b) ((a)^(b))
#define bitwise_not(a) (~(a))
#define bitwise_shift_left(a,n) ((a) << (n))
#define bitwise_shift_right(a,n) ((a) >> (n))

#define logical_and(a,b) ((a)&&(b))
#define logical_or(a,b) ((a)||(b))
#define logical_not(a) (!(a))

//#define A_DEBUG_PUTS
#ifdef A_DEBUG_PUTS
#define dputs_i(i) printf("%i\n",i);
#define dputs_s_i(s,i) printf("%s %i\n",s,i);
#endif


#define argc_msg(num,msg) if(argc < num){puts(msg); return 0;}
 
 
 //todo expr = 0;
 #define free_safe(expr) {if(expr) free(expr);}
 

 
/* verbose_printf */
//#define A_VERBOSE_PRINTF

#ifdef A_VERBOSE_PRINTF
#define verbose_printf printf
#else
#define verbose_printf(expr,...)
#endif

#endif


#ifndef STA_BYTES_H
#define STA_BYTES_H

//#include "STA/sta_def.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
#define STA_BYTES_IMPLEMENTATION
#include "sta-main/sta_bytes.h"
*/


void a_bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len);
void a_bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len);

void * a_bytes_copy_malloc(const void * bytes,int bytes_size);

void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2);

void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3);

 void * a_bytes_can_null_term_from_file_malloc(const char * file_name,a_bool be_null_term_output,long * out_size);
 
a_bool a_bytes_to_file(const char * file_name,const void * buffer,long buffer_size);
void * a_bytes_from_file_malloc(const char * file_name,long * out_size);
 void * a_bytes_from_file_part_malloc(const char * file_name,int offset,int size);
 
 void * a_bytes_pointer_at_offset(void * base,int offset);
void * a_bytes_malloc(void ** out_ptr_ptr,size_t size);
//void a_free(void * p);
#define a_free(p) {if(p) free(p);}
#define a_malloc(var,size) a_bytes_malloc((void**)&var,size)


 // void * a_bytes_from_file_header_malloc(const char * file_name,int size);
#define a_bytes_from_file_header_malloc(file_name,size) a_bytes_from_file_part_malloc(file_name,0,size)

char * a_string_from_file_malloc(const char * file_name);
a_bool a_string_to_file(const char * file_name,const char * text);

//
#define a_array_to_file(file_name,array_name,array_size,name_of_array_type) a_bytes_to_file(file_name,array_name,array_size * sizeof(name_of_array_type))


// var uint8_t * data; long data_size;
 #define a_data_bytes_from_file_malloc(filename) long data_size; uint8_t * data = a_bytes_from_file_malloc(filename,&data_size);

 
#define a_data_bytes_from_file_part_malloc(filename,offset,size) long data_size; uint8_t * data = a_bytes_from_file_part(filename,offset,size);



#define a_data_bytes_to_file(filename) a_bytes_to_file(filename,data,data_size);


// A Project By Arshad Latti
 


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


 void * a_bytes_can_null_term_from_file_malloc(const char * file_name,a_bool be_null_term_output,long * out_size)
{

	FILE * f = fopen(file_name,"rb");
	if(!f)
		return a_null;
		
	if(fseek(f,0,SEEK_END))
	{
		fclose(f);
		return a_null;
	}

	long size = ftell(f);
	
	void* buffer=a_null;
	char * text;

	
    if(size > 0)
	{
		if(be_null_term_output)
		size++;
	
		if(out_size)
	*out_size = size;
	
		if(!fseek(f,0,SEEK_SET))
		{
			
			buffer = malloc(size);
			if(buffer)
			{
				if(!fread(buffer,1,size,f))
				{
					free(buffer);
					fclose(f);
					return a_null;
				}
				if(be_null_term_output)
				{
					text = (char*)buffer;
					text[size - 1]=0;
				}
				
			}
		}
		
	}		
	fclose(f);	
	return buffer;
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


void * a_bytes_copy_malloc(const void * bytes,int bytes_size)
{
	if(!bytes_size) return 0;
	void * r = malloc(bytes_size);
	if(r)
	 memcpy(r,bytes,bytes_size);
return r;
}


void * a_bytes_from_file_malloc(const char * file_name,long * out_size)
{
	return a_bytes_can_null_term_from_file_malloc(file_name,a_false,out_size);
}




void * a_bytes_from_file_part_malloc(const char * file_name,int offset,int size)
{
		FILE * f = fopen(file_name,"rb");
	if(!f)
		return a_null;
		
	if(fseek(f,0,SEEK_END))
	{
		fclose(f);
		return a_null;
	}

	long file_size = ftell(f);
	
	
	void* buffer=a_null;
	char * text;

	
    if(file_size >= size + offset)
	{
		/* if(be_null_term_output)
		size++; */
	
		if(!fseek(f,offset,SEEK_SET))
		{
			
			buffer = malloc(size);
			if(buffer)
			{
				if(fread(buffer,1,size,f) != size)
				{
					free(buffer);
					fclose(f);
					return a_null;
				}
		
				
			}
		}
		
	}		
	fclose(f);

return buffer;	
}

void * a_bytes_malloc(void ** out_ptr_ptr,size_t size)
{
	*out_ptr_ptr = malloc(size);
	return *out_ptr_ptr;
}

void * a_bytes_pointer_at_offset(void * base,int offset)
{
	char * p = (char*)base;
	p+=offset;
	return p;
}

void a_bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
	char * p = (char*) bytes_src;
	p+=start;
	if(len)
		memcpy(bytes_dst,p,len);
}


a_bool a_bytes_to_file(const char * file_name,const void * buffer,long buffer_size)
{
	FILE * f=fopen(file_name,"wb");
	if(f)
	{
		if(fwrite(buffer,1,buffer_size,f) == buffer_size)
		{
		fclose(f);
		return a_true;
		}
		fclose(f);
	}
	
	return a_false;
}


void a_bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
		char * p = (char*) bytes_dst;
	p+=start;
	if(len)
		memcpy(p,bytes_src,len);
}


 char * a_string_from_file_malloc(const char * file_name)
{
	return (char * ) a_bytes_can_null_term_from_file_malloc(file_name,a_true,a_null);
}






a_bool a_string_to_file(const char * file_name,const char * text)
{
	return a_bytes_to_file(file_name,text,strlen(text));
}


#ifdef __cplusplus
}
#endif

#endif

#endif

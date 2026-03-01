
#ifdef STA_PUBLIC_API


/*
#define STA_STRING_IMPLEMENTATION
#include "sta-main/sta_string.h"
*/

//strings array functions
a_bool a_string_array_get_index_of(const char ** array,int array_size,const char * s_to_be_found);


//strings functions
char * a_string_from_char_array_malloc(const char * ca,int cc);
char * a_string_copy_malloc(const char * s);
char * a_string_copy_2_malloc(const char * s1,const char * s2);
char * a_string_copy_3_malloc(const char * s1,const char * s2,const char * s3);

char * a_string_from_w_string_malloc(const wchar_t * wc_str);
wchar_t * w_string_from_a_string_malloc(const char * s);
char * a_string_from_int_malloc(int v);

char * a_string_copy_digit0to9_only_malloc(const char * s);
char * a_string_copy_fixed_size_malloc(const char * s,const char c,int cc);

char * a_string_get_after_malloc(const char * s,const char * ss);
char * a_string_get_between_malloc(const char * s,const char * after,const char * stop_on);

//
char * a_string_get_after_ptr(const char * s,const char * ss);


//string search and compare functions
a_bool a_string_are_same(const char * a,const char * b);
a_bool a_string_is_valid(const char * s);
int a_index_of_char(const char * s,int c);
int a_index_of_string(const char * s,const char * ss);
a_bool a_string_is_begin_with(const char * s, const char * s_begin);
a_bool a_string_is_end_with(const char * s, const char * s_end);


//http 
char * a_url_from_http_GET_header_malloc( char * http_header);
char * a_url_from_http_HEAD_header_malloc( char * http_header);

//inplace 
void a_string_replace_char(char * s,char c,char new_c);
void a_string_replace_chars(char * s,const char * c,char new_c);
void w_string_replace_chars(wchar_t * s,const wchar_t * c,wchar_t new_c);
void a_string_keep_alphanum_and_replace(char * s,const char * keep,int c_any_other);

//bytes
uint8_t* a_bytes_from_hex_string_malloc(const char * hs,int extra_bytes,int * out_size);
char * a_hex_string_from_bytes_malloc(const uint8_t * mem,int mem_size);

//char functions
int a_char_hex_to_int(int c);

//text file 
a_bool a_text_read_line(char ** lines,char ** line,int * line_size);

//all lines reader


typedef struct
{
	char * text_all;
	a_bool text_all_is_malloc;
	char * lines;
	char * line;
	int line_size;
	int line_count;
} a_all_lines_t;

a_all_lines_t * a_all_lines_new_from_file(const char * filename);
a_all_lines_t * a_all_lines_new_from_string(char * s,a_bool do_malloc,a_bool is_malloc);
void a_all_lines_delete(a_all_lines_t * al);
a_bool a_all_lines_next(a_all_lines_t * al);
a_bool a_all_lines_next_peek(a_all_lines_t * al,char ** line,int * line_size);
a_bool a_all_lines_reset(a_all_lines_t * al);

// don't impact on reading lines
int a_all_lines_count(a_all_lines_t * al);
//var lines 
#define for_lines_file(filename) a_all_lines_t * lines = a_all_lines_new_from_file(filename); if(lines){ \
while(all_lines_next(lines)){
	
#define for_lines(txt) a_all_lines_t * lines = a_all_lines_new_from_string(txt,0,0); if(lines){ \
while(all_lines_next(lines)){
	
#define next_lines } all_lines_delete(lines); }


//for_line malloc and free  used auto
//var lines line_error empty_line line(likely used)
#define for_line_file(filename)  int line_error = 0;char * empty_line = "\0\0"; char * line; for_lines_file(filename)\
 if(lines->line_size) {line = a_string_from_char_array_malloc(lines->line,lines->line_size);\
 if(!line){line= empty_line; line_error = 1;} }\
else line = empty_line;
 
 
 
#define for_line(text)  int line_error = 0;char * empty_line = "\0\0"; char * line; for_lines(text)\
 if(lines->line_size) {line = a_string_from_char_array_malloc(lines->line,lines->line_size);\
 if(!line){line= empty_line; line_error = 1;}  }\
else line = empty_line;
 

	 
 #define next_line if(line != empty_line) free(line); next_lines

 // string to number
//2num (from string)
//int
#define a_num_i(i) atoi(i)
//uint 
#define a_num_u(u) strtoul(u,0,10)
//float atof(d)
#define a_num_f(s) strtof(s,0)
//double
#define a_num_d(d) strtod(d,0)

//64
#define a_num_i64(s) strtoll (s,0,10)
//  unsigned long long
#define a_num_u64(s) strtoull(s,0,10)

// scoped string 

//copy 
#define ssz__(expr) ssz__cdh(cdh,expr)
#define uuz__(expr) uuz__cdh(cdh,expr)

#define ssz_uuz(s) ssz__(a_string_from_w_string_malloc(s))
#define uuz_ssz(s) uuz__(w_string_from_a_string_malloc(s))

//char count
#define ssz_cc(s,cc) ssz__(a_string_from_char_array_malloc(s,cc))

#define ssz_1(psz) ssz__(a_string_copy_malloc(psz))
#define ssz_2(psz,psz2) ssz__(a_string_copy_2_malloc(psz,psz2))
#define ssz_3(psz,psz2,psz3) ssz__(a_string_copy_3_malloc(psz,psz2,psz3))
//#define ssz_4(psz,psz2,psz3,psz4) ssz__(a_string_copy_4_malloc(psz,psz2,psz3,psz4))

//load text file
#define ssz_file(filename) ssz__(a_string_from_file_malloc(filename))
//
#define ssz_s_cc(s,cc) ssz__(a_string_copy_fixed_size_malloc(s,'0',cc))

//todo 
#define ssz_clipboard(expr) ssz__


// var ssz__empty
char * ssz__cdh(a_cdh,char * s);
wchar_t * uuz__cdh(a_cdh,wchar_t * s);

 
#define uuz_same(s1,s2) (wcscmp(s1,s2)==0)
#define uuz_not_same(s1,s2) (wcscmp(s1,s2)!=0)












// A Project By Arshad Latti

#endif

#ifdef STA_STRING_IMPLEMENTATION
#define STA_BYTES_IMPLEMENTATION
#endif

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

#ifdef STA_STRING_IMPLEMENTATION
#define STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION
#endif

#ifdef STA_PUBLIC_API

/*
#define STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION
#include "sta-main/sta_context_dynamic_handler.h"
*/


typedef struct context_dynamic_handler_s context_dynamic_handler_t;

struct context_dynamic_handler_s
{
	context_dynamic_handler_t * sub;
	
	/* context_dynamic_handler_t * super;
	int sub_is_error; */
	
	int is_error;
	void * r;//return value
	gt_list_t * dh;
};


//note cdh parameter can null 
context_dynamic_handler_t * context_dynamic_handler_new(void);
context_dynamic_handler_t * context_dynamic_handler_sub(context_dynamic_handler_t * cdh);
void context_dynamic_handler_delete(context_dynamic_handler_t * cdh);

void * context_dynamic_handler_add(context_dynamic_handler_t * cdh,void * ptr,void * func);


//return pointer of var 
void * context_dynamic_handler_add_var(context_dynamic_handler_t * cdh,void ** var_ptr,void * func);

void * context_dynamic_handler_set_error(context_dynamic_handler_t * cdh,int is_error);


//var cdh
#define a_begin(expr) context_dynamic_handler_t * cdh = context_dynamic_handler_new();\
if(!cdh) return 0;

#define a_handle(ptr,func) if(ptr)\
{if(!context_dynamic_handler_add(cdh,ptr,func)) {(*func)(ptr); a_return(0)}}\
else a_return(0)
	
#define a_handle_var(var,func) if(!context_dynamic_handler_add_var(cdh,(void **)&var,func)){if(var)(*func)(var);a_return(0)}

#define a_handle_r(ptr,func) if(cdh){cdh->r = ptr; a_handle_var(cdh->r,func)}else{if(ptr)(*func)(ptr); a_return(0)}

#define a_return(expr) {context_dynamic_handler_delete(cdh);return expr;}
#define a_ok(expr) {if(!(expr)) a_return(0)}
#define a_ok_cdh(expr) {if(cdh){ if(cdh->is_error); a_return(0)}}

#define a_return_ok(expr) {if(cdh) cdh->r = 0;context_dynamic_handler_delete(cdh);return expr;}

#define a_sub context_dynamic_handler_sub(cdh)
#define a_cdh context_dynamic_handler_t * cdh
/* 
//example a_cdh and a_sub( both can null)
int cdh_foo(a_cdh)
{
	//use a_cdh
}
#define foo(expr) cdh_foo(a_sub) 

//example a_begin and a_return
int fun(void)
{
	a_begin()
	
	cdh_foo(a_sub);
	foo()
	
	a_return(0)
}


*/

#define a_create_pod(name_of_type,name_of_variable) name_of_type * name_of_variable = (name_of_type *) malloc(sizeof(name_of_type)); a_handle(name_of_variable,free) \
memset(name_of_variable,0,sizeof(name_of_type));

// example: a_new_context_dynamic_handler_t a_delete_context_dynamic_handler_t
// format : {a_new_}name_of_type {a_delete_}name_of_type
#define a_create(name_of_type,name_of_variable) a_create_(name_of_type,(),name_of_variable)
//a_create_(a,(),v)
#define a_create_(name_of_type,parameters,name_of_variable) name_of_type * name_of_variable = a_new##name_of_type parameters;\
a_handle(name_of_variable,a_delete##name_of_type)


#define a_new(name_of_type,name_of_variable) a_new_(name_of_type,(),name_of_variable)

#define a_new_(name_of_type,parameters,name_of_variable)name_of_variable = a_new##name_of_type parameters;\
a_handle(name_of_variable,a_delete##name_of_type)



// A Project By Arshad Latti

#endif

#ifdef STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION
#define STA_GT_LIST_IMPLEMENTATION
#endif

#ifdef STA_PUBLIC_API


//#define STA_GT_LIST_IMPLEMENTATION
//#include "sta-main/sta_gt_list.h"


struct s_gt_list__node
{
	struct s_gt_list__node * next;
	struct s_gt_list__node * pre;
	gt_term_free_func data_term_func;
	char data[sizeof(struct s_gt_list__node *)];
};
typedef struct s_gt_list__node  gt_list__node_t;

struct s_gt_list
{
	gt_list__node_t * node_first;
	gt_list__node_t * node_last;
	
	size_t item_size;
	size_t items_count;
	
	gt_init_with_func item_init_with_func;
	gt_term_free_func item_term_free_func;
};
typedef struct s_gt_list gt_list_t;



gt_list_t * gt_list_new_ex(size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
gt_list_t * gt_list_new(size_t item_size);
void gt_list_delete(gt_list_t * gtl);

a_bool gt_list_add_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
a_bool gt_list_add(gt_list_t * gtl,void * item);

a_bool gt_list_add_first(gt_list_t * gtl,void * item);
a_bool gt_list_add_first_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);


gt_list__node_t * gt_list_add_at(gt_list_t * gtl,void * item,int index);
gt_list__node_t * gt_list_add_at_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func,int index);


void * gt_list_get_at(gt_list_t * gtl,int index);
//a_bool gt_list_add_first_ex



gt_list__node_t * gt_list__node_add(gt_list_t * gtl,size_t item_size,a_bool is_at_first);
gt_list__node_t * gt_list__node_remove(gt_list_t * gtl,a_bool is_at_first);

gt_list__node_t * gt_list__node_add_at(gt_list_t * gtl,size_t item_size,int index);
gt_list__node_t * gt_list__node_get_at(gt_list_t * gtl,int index);


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


#ifndef STA_GENERAL_TYPES_H
#define STA_GENERAL_TYPES_H

/* #include "sta_def.h"
#include <stddef.h>


 */
 
 #ifdef __cplusplus
extern "C" {
#endif
 
/*
gt_init_with_func
gt_term_free_func
gt_set_func // line a= b
gt_init_func//CCls c();

gt_is_equual_func
gt_is_greater_then_func
gt_is_less_then_func
gt_is_less_or_equle_func
gt_is_greater_or_equle_func
gt_append_func //like strcat
gt_math_add_func // math_add(out,a,b);
sub/div/mul etc..
*/


typedef void * (*gt_init_with_func)(void * dst,void * src);//return dst ptr on success
typedef void  (*gt_term_free_func)(void * ptr);
typedef void (*gt_set_func)(void * dst,void * src);
typedef void (*gt_init_func)(void * ptr);


#ifdef __cplusplus
}
#endif






#endif


#ifndef STA_GT_LIST_H
#define STA_GT_LIST_H

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


//#define STA_GT_LIST_IMPLEMENTATION
//#include "sta-main/sta_gt_list.h"


struct s_gt_list__node
{
	struct s_gt_list__node * next;
	struct s_gt_list__node * pre;
	gt_term_free_func data_term_func;
	char data[sizeof(struct s_gt_list__node *)];
};
typedef struct s_gt_list__node  gt_list__node_t;

struct s_gt_list
{
	gt_list__node_t * node_first;
	gt_list__node_t * node_last;
	
	size_t item_size;
	size_t items_count;
	
	gt_init_with_func item_init_with_func;
	gt_term_free_func item_term_free_func;
};
typedef struct s_gt_list gt_list_t;



gt_list_t * gt_list_new_ex(size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
gt_list_t * gt_list_new(size_t item_size);
void gt_list_delete(gt_list_t * gtl);

a_bool gt_list_add_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
a_bool gt_list_add(gt_list_t * gtl,void * item);

a_bool gt_list_add_first(gt_list_t * gtl,void * item);
a_bool gt_list_add_first_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);


gt_list__node_t * gt_list_add_at(gt_list_t * gtl,void * item,int index);
gt_list__node_t * gt_list_add_at_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func,int index);


void * gt_list_get_at(gt_list_t * gtl,int index);
//a_bool gt_list_add_first_ex



gt_list__node_t * gt_list__node_add(gt_list_t * gtl,size_t item_size,a_bool is_at_first);
gt_list__node_t * gt_list__node_remove(gt_list_t * gtl,a_bool is_at_first);

gt_list__node_t * gt_list__node_add_at(gt_list_t * gtl,size_t item_size,int index);
gt_list__node_t * gt_list__node_get_at(gt_list_t * gtl,int index);



void * sta_malloc_ex(void ** out_ptr_ptr,size_t size);
#define sta_malloc(var,size) sta_malloc_ex((void**)var,size)

#ifdef __cplusplus
}
#endif

#endif




#ifdef STA_GT_LIST_IMPLEMENTATION

#ifndef STA_GT_LIST_IMPLEMENTATION_INCLUDED
#define STA_GT_LIST_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif



a_bool gt_list_add(gt_list_t * gtl,void * item)
{
	return gt_list_add_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func);
}

gt_list__node_t * gt_list_add_at(gt_list_t * gtl,void * item,int index)
{
	
		return gt_list_add_at_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func,index);

	
}


gt_list__node_t * gt_list_add_at_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func,int index)
{
	gt_list__node_t * node = gt_list__node_add_at(gtl,item_size,index);
	if(node)
	{
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	}
	return node;
}







a_bool gt_list_add_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list__node_t * node = gt_list__node_add(gtl,item_size,a_false);
	if(!node)
		return a_false;
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	
	return a_true;
	
}





a_bool gt_list_add_first(gt_list_t * gtl,void * item)
{
	return gt_list_add_first_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func);
}



a_bool gt_list_add_first_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list__node_t * node = gt_list__node_add(gtl,item_size,a_true);
	if(!node)
		return a_false;
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	return a_true;
}



void gt_list_delete(gt_list_t * gtl)
{
	gt_list__node_t * node;
	while((node = gt_list__node_remove(gtl,a_false)))
	{
		if(node->data_term_func)
			(*node->data_term_func)(node->data);
		free(node);
	}
	free(gtl);
}


void * gt_list_get_at(gt_list_t * gtl,int index)
{
	
	gt_list__node_t * node= gt_list__node_get_at(gtl,index);
	if(node)
	{
		
		return node->data;
	}
	
	return a_null;
}


gt_list_t * gt_list_new(size_t item_size)
{
	return gt_list_new_ex(item_size,a_null,a_null);
}



gt_list_t * gt_list_new_ex(size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list_t * gtl;
	sta_malloc(&gtl,sizeof(gt_list_t));
	if(!gtl)
		return a_null;
	gtl->node_first = a_null;
	gtl->node_last  = a_null;
	gtl->item_size  = item_size;
	gtl->items_count=0;
	gtl->item_init_with_func = item_init_with_func;
	gtl->item_term_free_func = item_term_free_func;
	return gtl;//FIXED  not return gtl
}


gt_list__node_t * gt_list__node_add(gt_list_t * gtl,size_t item_size,a_bool is_at_first)
{
	gt_list__node_t * node;
	if(!sta_malloc(&node,sizeof(gt_list__node_t) + (item_size- sizeof(gt_list__node_t*))))
		return a_null;
	
	gtl->items_count++;
	if(gtl->items_count == 1)//if list is empty
	{
		gtl->node_first = node;
		gtl->node_last  = node;
		return node;
	}
	
	gt_list__node_t * node_pre;
	
	if(is_at_first)
	{
	node_pre = gtl->node_first;
	node_pre->pre = node;
	node->pre = a_null;
	node->next = node_pre;
	gtl->node_first = node;
	}
    else
	{
	node_pre = gtl->node_last;
	node_pre->next=node;
	node->pre = node_pre;
	node->next = a_null;
	gtl->node_last = node;
	}


	return node;
}




gt_list__node_t * gt_list__node_add_at(gt_list_t * gtl,size_t item_size,int index)
{
	if(index == 0)
		return gt_list__node_add(gtl,item_size,a_true);
	if(index >= gtl->items_count)
		return gt_list__node_add(gtl,item_size,a_false);
	
	
	gt_list__node_t * node;
	
	   
	gt_list__node_t * node_pre;
    node_pre = gt_list__node_get_at(gtl,index);
	
	if(!node_pre)
		return a_null;
	
	if(!sta_malloc(&node,sizeof(gt_list__node_t) + (item_size- sizeof(gt_list__node_t*))))
		return a_null;
	
    node->pre = node_pre->pre;
	node->next = node_pre;
	
	node_pre->pre = node;
   node->pre->next = node;
	gtl->items_count++;

	return node;
	
}




gt_list__node_t * gt_list__node_get_at(gt_list_t * gtl,int index)
{
		gt_list__node_t * node;
	if(!gtl->items_count)
		return a_null;
	if( index >= gtl->items_count)
		return a_null;
	int i = 0;
	node  = gtl->node_first;
	//if(index == 0)
		//return node;
	for(i=0;i< gtl->items_count;i++)
	{
			if(index == i)
			return node;
			node = node->next;
	if(!node)
		return a_null;
	}
	
	
	
	
	return a_null;
}



gt_list__node_t * gt_list__node_remove(gt_list_t * gtl,a_bool is_at_first)
{
	gt_list__node_t * node;
	
	if(!gtl->items_count)
		return a_null;
	
	if(gtl->items_count == 1)
	{
		node = gtl->node_first;
		gtl->items_count=0;
		return node;
	}
	
	gtl->items_count--;
	
	if(is_at_first)
	{
		node = gtl->node_first;
		gtl->node_first = node->next;
	}
	else
	{
		node = gtl->node_last;
		gtl->node_last = node->pre;
	}
	return node;
}



void * sta_malloc_ex(void ** out_ptr_ptr,size_t size)
{
	void * mem = malloc(size);
	if(out_ptr_ptr)
		*out_ptr_ptr = mem;
	
	return mem;
}


#ifdef __cplusplus
}
#endif

#endif

#endif




#ifndef STA_CONTEXT_DYNAMIC_HANDLER_H
#define STA_CONTEXT_DYNAMIC_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

struct s_context_dynamic_handler__item
{
	void * ptr;
	gt_term_free_func term_func;
	a_bool is_var;
};
typedef struct s_context_dynamic_handler__item context_dynamic_handler__item_t;
void context_dynamic_handler__item_term(void * p);

/*
#define STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION
#include "sta-main/sta_context_dynamic_handler.h"
*/


typedef struct context_dynamic_handler_s context_dynamic_handler_t;

struct context_dynamic_handler_s
{
	context_dynamic_handler_t * sub;
	
	/* context_dynamic_handler_t * super;
	int sub_is_error; */
	
	int is_error;
	void * r;//return value
	gt_list_t * dh;
};


//note cdh parameter can null 
context_dynamic_handler_t * context_dynamic_handler_new(void);
context_dynamic_handler_t * context_dynamic_handler_sub(context_dynamic_handler_t * cdh);
void context_dynamic_handler_delete(context_dynamic_handler_t * cdh);

void * context_dynamic_handler_add(context_dynamic_handler_t * cdh,void * ptr,void * func);


//return pointer of var 
void * context_dynamic_handler_add_var(context_dynamic_handler_t * cdh,void ** var_ptr,void * func);

void * context_dynamic_handler_set_error(context_dynamic_handler_t * cdh,int is_error);


//var cdh
#define a_begin(expr) context_dynamic_handler_t * cdh = context_dynamic_handler_new();\
if(!cdh) return 0;

#define a_handle(ptr,func) if(ptr)\
{if(!context_dynamic_handler_add(cdh,ptr,func)) {(*func)(ptr); a_return(0)}}\
else a_return(0)
	
#define a_handle_var(var,func) if(!context_dynamic_handler_add_var(cdh,(void **)&var,func)){if(var)(*func)(var);a_return(0)}

#define a_handle_r(ptr,func) if(cdh){cdh->r = ptr; a_handle_var(cdh->r,func)}else{if(ptr)(*func)(ptr); a_return(0)}

#define a_return(expr) {context_dynamic_handler_delete(cdh);return expr;}
#define a_ok(expr) {if(!(expr)) a_return(0)}
#define a_ok_cdh(expr) {if(cdh){ if(cdh->is_error); a_return(0)}}

#define a_return_ok(expr) {if(cdh) cdh->r = 0;context_dynamic_handler_delete(cdh);return expr;}

#define a_sub context_dynamic_handler_sub(cdh)
#define a_cdh context_dynamic_handler_t * cdh
/* 
//example a_cdh and a_sub( both can null)
int cdh_foo(a_cdh)
{
	//use a_cdh
}
#define foo(expr) cdh_foo(a_sub) 

//example a_begin and a_return
int fun(void)
{
	a_begin()
	
	cdh_foo(a_sub);
	foo()
	
	a_return(0)
}


*/

#define a_create_pod(name_of_type,name_of_variable) name_of_type * name_of_variable = (name_of_type *) malloc(sizeof(name_of_type)); a_handle(name_of_variable,free) \
memset(name_of_variable,0,sizeof(name_of_type));

// example: a_new_context_dynamic_handler_t a_delete_context_dynamic_handler_t
// format : {a_new_}name_of_type {a_delete_}name_of_type
#define a_create(name_of_type,name_of_variable) a_create_(name_of_type,(),name_of_variable)
//a_create_(a,(),v)
#define a_create_(name_of_type,parameters,name_of_variable) name_of_type * name_of_variable = a_new##name_of_type parameters;\
a_handle(name_of_variable,a_delete##name_of_type)


#define a_new(name_of_type,name_of_variable) a_new_(name_of_type,(),name_of_variable)

#define a_new_(name_of_type,parameters,name_of_variable)name_of_variable = a_new##name_of_type parameters;\
a_handle(name_of_variable,a_delete##name_of_type)



// A Project By Arshad Latti


#ifdef __cplusplus
}
#endif

#endif





#ifdef STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION

#ifndef STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION_INCLUDED
#define STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void * context_dynamic_handler_add(context_dynamic_handler_t * cdh,void * ptr,void * func)
{
	
		if(!cdh) return a_null;
	
	
	context_dynamic_handler__item_t  item;
	item.ptr = ptr;
	item.is_var = a_false;
	item.term_func = (gt_term_free_func)func;
	if(gt_list_add(cdh->dh,&item))
		return ptr;
 return a_null;
}

void * context_dynamic_handler_add_var(context_dynamic_handler_t * cdh,void ** var_ptr,void * func){
	
	if(!cdh) return a_null;
	
		context_dynamic_handler__item_t item;
	item.ptr = (void*)var_ptr;
	item.is_var = a_true;
	item.term_func = (gt_term_free_func)func;
	if(gt_list_add(cdh->dh,&item))
		return (void*)var_ptr;
 return a_null;
}

void context_dynamic_handler_delete(context_dynamic_handler_t * cdh)
{
	if(cdh)
	{
	 if(cdh->sub)
       context_dynamic_handler_delete(cdh->sub);
      gt_list_delete(cdh->dh);
	  free(cdh);
	}
}

context_dynamic_handler_t * context_dynamic_handler_new(void)
{
	context_dynamic_handler_t * r = (context_dynamic_handler_t *) malloc(sizeof(context_dynamic_handler_t));
	if(r)
	{
		
	
	r->dh = gt_list_new_ex(sizeof(context_dynamic_handler__item_t),a_null,context_dynamic_handler__item_term);
	if(!r->dh)
	{
		free(r);
		return a_null;
	}
	r->is_error=0;
	r->r=a_null;
	r->sub= a_null;
	
	}
	
	return r;
}


void * context_dynamic_handler_set_error(context_dynamic_handler_t * cdh,int is_error)
{
	
	if(cdh)
	{
		cdh->is_error = is_error;
	}
}

context_dynamic_handler_t * context_dynamic_handler_sub(context_dynamic_handler_t * cdh)
{
	if(cdh)
	{
		if(cdh->sub)
			context_dynamic_handler_delete(cdh->sub);
		cdh->sub = context_dynamic_handler_new();
		return cdh->sub;
	}
	return a_null;
}


void context_dynamic_handler__item_term(void * p)
{
	
	context_dynamic_handler__item_t * itm=(context_dynamic_handler__item_t*)p;
	void ** ptr;
	if(!itm)
		return;
	
	if(!itm->term_func)
		return;
	
	if(!itm->ptr)
		return;
	
	if(itm->is_var)
	{
		ptr = (void**)itm->ptr;
		if(*ptr)(*itm->term_func)(*ptr); //	(*itm->term_func)(*ptr);
	}
	else
	{
		(*itm->term_func)(itm->ptr);
	}
	
}



#ifdef __cplusplus
}
#endif

#endif

#endif




#ifndef STA_STRING_H
#define STA_STRING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif




/*
#define STA_STRING_IMPLEMENTATION
#include "sta-main/sta_string.h"
*/

//strings array functions
a_bool a_string_array_get_index_of(const char ** array,int array_size,const char * s_to_be_found);


//strings functions
char * a_string_from_char_array_malloc(const char * ca,int cc);
char * a_string_copy_malloc(const char * s);
char * a_string_copy_2_malloc(const char * s1,const char * s2);
char * a_string_copy_3_malloc(const char * s1,const char * s2,const char * s3);

char * a_string_from_w_string_malloc(const wchar_t * wc_str);
wchar_t * w_string_from_a_string_malloc(const char * s);
char * a_string_from_int_malloc(int v);

char * a_string_copy_digit0to9_only_malloc(const char * s);
char * a_string_copy_fixed_size_malloc(const char * s,const char c,int cc);

char * a_string_get_after_malloc(const char * s,const char * ss);
char * a_string_get_between_malloc(const char * s,const char * after,const char * stop_on);

//
char * a_string_get_after_ptr(const char * s,const char * ss);


//string search and compare functions
a_bool a_string_are_same(const char * a,const char * b);
a_bool a_string_is_valid(const char * s);
int a_index_of_char(const char * s,int c);
int a_index_of_string(const char * s,const char * ss);
a_bool a_string_is_begin_with(const char * s, const char * s_begin);
a_bool a_string_is_end_with(const char * s, const char * s_end);


//http 
char * a_url_from_http_GET_header_malloc( char * http_header);
char * a_url_from_http_HEAD_header_malloc( char * http_header);

//inplace 
void a_string_replace_char(char * s,char c,char new_c);
void a_string_replace_chars(char * s,const char * c,char new_c);
void w_string_replace_chars(wchar_t * s,const wchar_t * c,wchar_t new_c);
void a_string_keep_alphanum_and_replace(char * s,const char * keep,int c_any_other);

//bytes
uint8_t* a_bytes_from_hex_string_malloc(const char * hs,int extra_bytes,int * out_size);
char * a_hex_string_from_bytes_malloc(const uint8_t * mem,int mem_size);

//char functions
int a_char_hex_to_int(int c);

//text file 
a_bool a_text_read_line(char ** lines,char ** line,int * line_size);

//all lines reader


typedef struct
{
	char * text_all;
	a_bool text_all_is_malloc;
	char * lines;
	char * line;
	int line_size;
	int line_count;
} a_all_lines_t;

a_all_lines_t * a_all_lines_new_from_file(const char * filename);
a_all_lines_t * a_all_lines_new_from_string(char * s,a_bool do_malloc,a_bool is_malloc);
void a_all_lines_delete(a_all_lines_t * al);
a_bool a_all_lines_next(a_all_lines_t * al);
a_bool a_all_lines_next_peek(a_all_lines_t * al,char ** line,int * line_size);
a_bool a_all_lines_reset(a_all_lines_t * al);

// don't impact on reading lines
int a_all_lines_count(a_all_lines_t * al);
//var lines 
#define for_lines_file(filename) a_all_lines_t * lines = a_all_lines_new_from_file(filename); if(lines){ \
while(all_lines_next(lines)){
	
#define for_lines(txt) a_all_lines_t * lines = a_all_lines_new_from_string(txt,0,0); if(lines){ \
while(all_lines_next(lines)){
	
#define next_lines } all_lines_delete(lines); }


//for_line malloc and free  used auto
//var lines line_error empty_line line(likely used)
#define for_line_file(filename)  int line_error = 0;char * empty_line = "\0\0"; char * line; for_lines_file(filename)\
 if(lines->line_size) {line = a_string_from_char_array_malloc(lines->line,lines->line_size);\
 if(!line){line= empty_line; line_error = 1;} }\
else line = empty_line;
 
 
 
#define for_line(text)  int line_error = 0;char * empty_line = "\0\0"; char * line; for_lines(text)\
 if(lines->line_size) {line = a_string_from_char_array_malloc(lines->line,lines->line_size);\
 if(!line){line= empty_line; line_error = 1;}  }\
else line = empty_line;
 

	 
 #define next_line if(line != empty_line) free(line); next_lines

 // string to number
//2num (from string)
//int
#define a_num_i(i) atoi(i)
//uint 
#define a_num_u(u) strtoul(u,0,10)
//float atof(d)
#define a_num_f(s) strtof(s,0)
//double
#define a_num_d(d) strtod(d,0)

//64
#define a_num_i64(s) strtoll (s,0,10)
//  unsigned long long
#define a_num_u64(s) strtoull(s,0,10)

// scoped string 

//copy 
#define ssz__(expr) ssz__cdh(cdh,expr)
#define uuz__(expr) uuz__cdh(cdh,expr)

#define ssz_uuz(s) ssz__(a_string_from_w_string_malloc(s))
#define uuz_ssz(s) uuz__(w_string_from_a_string_malloc(s))

//char count
#define ssz_cc(s,cc) ssz__(a_string_from_char_array_malloc(s,cc))

#define ssz_1(psz) ssz__(a_string_copy_malloc(psz))
#define ssz_2(psz,psz2) ssz__(a_string_copy_2_malloc(psz,psz2))
#define ssz_3(psz,psz2,psz3) ssz__(a_string_copy_3_malloc(psz,psz2,psz3))
//#define ssz_4(psz,psz2,psz3,psz4) ssz__(a_string_copy_4_malloc(psz,psz2,psz3,psz4))

//load text file
#define ssz_file(filename) ssz__(a_string_from_file_malloc(filename))
//
#define ssz_s_cc(s,cc) ssz__(a_string_copy_fixed_size_malloc(s,'0',cc))

//todo 
#define ssz_clipboard(expr) ssz__


// var ssz__empty
char * ssz__cdh(a_cdh,char * s);
wchar_t * uuz__cdh(a_cdh,wchar_t * s);

 
#define uuz_same(s1,s2) (wcscmp(s1,s2)==0)
#define uuz_not_same(s1,s2) (wcscmp(s1,s2)!=0)












// A Project By Arshad Latti




#ifdef __cplusplus
}
#endif

#endif


#ifdef STA_STRING_IMPLEMENTATION

#ifndef STA_STRING_IMPLEMENTATION_INCLUDED
#define STA_STRING_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

int a_all_lines_count(a_all_lines_t * al)
{
	a_all_lines_t a;
	int r=0;
	a.lines = al->text_all;
	a.line_count = 0;
	while(a_all_lines_next(&a)) r++;
	return r;
}

void a_all_lines_delete(a_all_lines_t * al)
{
	if(al->text_all_is_malloc)
		free(al->text_all);
	free(al);
}

a_all_lines_t * a_all_lines_new_from_file(const char * filename)
{
	char * text_all = a_string_from_file_malloc(filename);
 

if(text_all)
	return a_all_lines_new_from_string(text_all,a_false,a_true);

return a_null;
}


a_all_lines_t * a_all_lines_new_from_string( char * s,a_bool do_malloc,a_bool is_malloc)
{
	a_all_lines_t * r = malloc(sizeof(a_all_lines_t));

	if(r)
	{
		if(do_malloc)
		{
		r->text_all= a_string_copy_malloc(s);
		if(!r->text_all)
		{
			free(r);
			return a_null;
		}
		r->text_all_is_malloc = a_true;
		r->lines = r->text_all;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		else
		{
		r->text_all= s;
		r->text_all_is_malloc = is_malloc;
		r->lines = s;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		
		
	}
	return r;
}



a_bool a_all_lines_next(a_all_lines_t * al)
{
	a_bool r;
	r = a_text_read_line(&al->lines,&al->line,&al->line_size);
	if(r) al->line_count++;
	return r;
}




a_bool a_all_lines_next_peek(a_all_lines_t * al,char ** line,int * line_size)
{
	char * lines = al->lines;
	return a_text_read_line(&lines,line,line_size);
}


a_bool a_all_lines_reset(a_all_lines_t * al)
{
		al->lines = al->text_all;
		al->line_count = 0;
}



uint8_t* a_bytes_from_hex_string_malloc(const char * hs,int extra_bytes,int * out_size)
{
	int binary_size = strlen(hs) / 2 ;
	if(!binary_size) return 0;
	int bytes_count = binary_size+ extra_bytes;
	
	uint8_t* r = (uint8_t*)malloc(bytes_count);
	
	if(r)
	{
		if(out_size) *out_size = bytes_count;
		
		memset(r,0,bytes_count);
		
		uint8_t bin;
		for_i(binary_size)
		{
			
			bin =a_char_hex_to_int(hs[i*2]) * 16;
			bin += a_char_hex_to_int(hs[(i*2)+1]);
			r[i] = bin;
		}
	}
		
	
	return r;
}


// 0..9 and A..F  to 0..15
int a_char_hex_to_int(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return (c - 'A') + 10;
	
	if(c >= 'a' && c <= 'f')
		return (c - 'a') + 10;
	return 0;
}


char * a_hex_string_from_bytes_malloc(const uint8_t * mem,int mem_size)
{
	const char hx[]="0123456789ABCDEF";
	char * r = (char * ) malloc(mem_size*2+1);
	if(r)
	{
		for_i(mem_size)
		{
			r[i*2] = hx[mem[i] / 16];
			r[i*2+1] = hx[mem[i] % 16];
		}
		r[mem_size*2] = 0;
	}
	
	return r;
}

int a_index_of_char(const char * s,int c)
{
	for_i_cc(s){
		if(s[i]== c) return i;
	}
	
	return -1;
}

int a_index_of_string(const char * s,const char * ss)
{
	char * p = strstr(s,ss);
	if(p)
	{
		return p - s;
	}
	
	return -1;
}


a_bool a_string_are_same(const char * a,const char * b)
{
	int cca = strlen(a);
	int ccb = strlen(b);
	if(cca == ccb)
	{
		for_i(cca)
		{
			if(a[i] != b[i])
				return a_false;
		}
		return a_true;
	}
	return a_false;
}

// >= 0 mean valid and on -1 mean not found.
a_bool a_string_array_get_index_of(const char ** array,int array_size,const char * s_to_be_found)
{
	for_i(array_size)
	{
	     if(a_string_are_same(array[i],s_to_be_found)) return i;
	}
	
	return -1;//a_not_found
}



char * a_string_copy_2_malloc(const char * s1,const char * s2)
{
		char * r = (char*) malloc(strlen(s1)+strlen(s2)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		}
		return r;		
}


char * a_string_copy_3_malloc(const char * s1,const char * s2,const char * s3)
{
			char * r = (char*) malloc(strlen(s1)+strlen(s2)+strlen(s3)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		strcat(r,s3);
		}
		return r;		
}


char * a_string_copy_digit0to9_only_malloc(const char * s)
{
	
	int cc = strlen(s);
	 char * r=(char * )malloc(cc+1);
	 int dd=0;
	if(r)
	{
		//puts_s_i("count",cc);
		for_i(cc)
		{
		//	putch(s[i]);
			if(s[i] >='0' && s[i] <='9')
			{
				
				r[dd]=s[i];
				dd++;
			}
		}
		r[dd]=0;
		
	}
	return r;
}

// cc  = 3 s = "1" c = '0' return value ="001"
char * a_string_copy_fixed_size_malloc(const char * s,const char c,int cc)
{
	int l = strlen(s);
	int missed = cc - l;
	char * r =(char*)malloc(cc+l+1);//cc to missed
	char * p = r;
	if(r)
	{
		for_i(missed)
		{
			p[0] = c;
			p++;
		}
		strcpy(p,s);
	}
	
	return r;
}

char * a_string_copy_malloc(const char * s)
{
	return a_string_from_char_array_malloc(s,strlen(s));
}


char * a_string_copy_stop_at_char_malloc(const char * s,int c)
{
	char * path_e = a_string_copy_malloc(s);
	char * r = 0;
	char * p;
	if(path_e)
	{
		p = path_e;
		while(*p)
		{
			if(p[0] == c)
			{
				p[0]=0;
				break;
			}
			p++;
		}
		r = a_string_copy_malloc(path_e);
		free(path_e);
	}
	return r;
}


char * a_string_from_char_array_malloc(const char * ca,int cc)
{
	char * r = (char*)malloc(cc+1);
	if(r)
	{
		memcpy(r,ca,cc);
	r[cc]=0;
	}
 return r;
}


char * a_string_from_int_malloc(int v)
{
	// sprintf(str,"%d",value) converts to decimal base.
// sprintf(str,"%x",value) converts to hexadecimal base.
// sprintf(str,"%o",value) converts to octal base.

	char str[80];
	//itoa(v,str,10);
	sprintf(str,"%d",v);
	return a_string_copy_malloc(str);
}



char * a_string_from_w_string_malloc(const wchar_t * wc_str)
{
	int n = wcslen(wc_str);
	//char * c;
	//n++;
	char * r = (char*) malloc((n+1));
	
if(r)
{
	r[n]=0;
	for_i(n)
	{
		r[i]=wc_str[i];
	}
}
	return r;
}

char * a_string_get_after_malloc(const char * s,const char * ss)
{
	char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return a_string_copy_malloc(&s[i+strlen(ss)]);
	}
	return r;
}


char * a_string_get_after_ptr(const char * s,const char * ss)
{
	//char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return (char*)&s[i+strlen(ss)];
	}
	return a_null;
}

char * a_string_get_between_malloc(const char * s,const char * after,const char * stop_on)
{
	char * r = a_string_get_after_malloc(s,after);
	if(r)
	{
		
		int i = a_index_of_string(r,stop_on);
		if(i  >= 0)
		{
			r[i]=0;
			return r;
		}
		
		free(r);
		return a_null;
	}

	return r;
}



a_bool a_string_is_begin_with(const char * s, const char * s_begin)
{

	if(strstr(s,s_begin) == s) return a_true;

return a_false;
}

a_bool a_string_is_end_with(const char * s, const char * s_end)
{
	int cc = strlen(s);
	int cc_end = strlen(s_end);
if(cc >= cc_end)
{
	const char * begin = &s[cc - cc_end];
	if(strstr(begin,s_end) == begin) return a_true;
}
return a_false;
}



a_bool a_string_is_valid(const char * s)
{
	if(s)
	{
		if(*s)
			return a_true;
	}
	return a_false;
}


void a_string_keep_alphanum_and_replace(char * s,const char * keep,int c_any_other)
{
			int cc = strlen(s);
		for_i(cc)
		{
			if(!isalnum(s[i]) && a_index_of_char(keep,s[i]) < 0 )
				s[i] = c_any_other;
		}
}



void a_string_replace_char(char * s,char c,char new_c)
{
	while(*s)
	{
		
		if(*s == c) *s = new_c;
		s++;
	}
}

void a_string_replace_chars(char * s,const char * c,char new_c)
{
	const char * f;
	while(*s)
	{
		f=c;
		while(*f)
		{
		if(*s == *f) *s = new_c;
		f++;
		}
		s++;
	}
}

a_bool a_text_read_line(char ** lines,char ** line,int * line_size)
{
	int cc=0;
	char * text=*lines;
	*line = text;
	if(text[0] ==0)
		return a_false;
	
	while(*text)
	{
		if(*text == '\n' || *text == '\r')
		{
			if(*text == '\r')
			{
				text++;
				continue;
			}
			
			text++;
			*lines = text;
			*line_size = cc;
			return a_true;
		}
		cc++;
		text++;
		if(*text == 0)
		{
			*lines = text;
			*line_size = cc;
			return a_true;
		}
	}
	
	
	return a_false;
}



char * a_url_from_http_GET_header_malloc( char * http_header)
{
	const char * uri_begin;
	char * uri_end;
	char * url=a_null;
	uri_begin=strstr(http_header,"GET ");
	if(uri_begin && uri_begin == http_header)
	{
		uri_begin+=4;
		if(uri_end=strstr(uri_begin," "))
		{
			int len = uri_end - uri_begin;
			if(len >0);
			url = a_string_from_char_array_malloc(uri_begin,len);
		}
	}
	return url;
}




char * a_url_from_http_HEAD_header_malloc( char * http_header)
{
	const char * uri_begin;
	char * uri_end;
	char * url=a_null;
	
	uri_begin=strstr(http_header,"HEAD ");
	if(uri_begin && uri_begin == http_header)
	{
		uri_begin+=5;
		if(uri_end=strstr(uri_begin," "))
		{
			int len = uri_end - uri_begin;
			if(len >0);
			url = a_string_from_char_array_malloc(uri_begin,len);
		}
	}
	return url;
}



char  ssz__empty[4]={0};
//s from malloc
char * ssz__cdh(a_cdh,char * s)
{
	if(s)
	{
		char * r = (char*)context_dynamic_handler_add(cdh,s,(void*)free);
	if(!r) 
	{
		context_dynamic_handler_set_error(cdh,a_true);
		free(s);
	}
	else 
		return r;
	}
	else 
	{
		context_dynamic_handler_set_error(cdh,a_true);
	}
	
	return ssz__empty;
}

wchar_t  uuz__empty[4]={0};
wchar_t * uuz__cdh(a_cdh,wchar_t * s)
{
	
		if(s)
	{
		wchar_t * r = (wchar_t*)context_dynamic_handler_add(cdh,s,(void*)free);
	if(!r) 
	{
		context_dynamic_handler_set_error(cdh,a_true);
		free(s);
	}
	else 
		return r;
	}
	else 
	{
		context_dynamic_handler_set_error(cdh,a_true);
	}
	
	return uuz__empty;
}




wchar_t * w_string_from_a_string_malloc(const char * s)
{
	int n = strlen(s);
	char * c;
	//n++;
	uint16_t * r = (uint16_t*) malloc((n+1)*2);
	if(r)
	{
		r[n]=0;
	//int i;
	for_i(n)
	{
		r[i]=0;//for zero upper byte
		c=(char*)&r[i];
		*c=s[i];
	}
	
	}
	return (wchar_t*)r;
	
}


void w_string_replace_chars(wchar_t * s,const wchar_t * c,wchar_t new_c)
{
	const wchar_t * f;
	while(*s)
	{
		f=c;
		while(*f)
		{
		if(*s == *f) *s = new_c;
		f++;
		}
		s++;
	}
}


#ifdef __cplusplus
}
#endif

#endif

#endif


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

// A Project By Arshad Latti
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

#include "a_string.c"

#ifdef __cplusplus
}
#endif

#endif

#endif





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

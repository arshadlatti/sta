
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
 

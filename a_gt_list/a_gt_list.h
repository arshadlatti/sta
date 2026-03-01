
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


// A Project By Arshad Latti


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

#include "a_gt_list.c"

#ifdef __cplusplus
}
#endif

#endif

#endif

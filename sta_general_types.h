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

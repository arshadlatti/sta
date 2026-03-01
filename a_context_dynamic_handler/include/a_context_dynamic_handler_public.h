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
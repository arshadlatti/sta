#define STA_CONTEXT_DYNAMIC_HANDLER_IMPLEMENTATION
#include "sta-main/sta_context_dynamic_handler.h"
#define STA_STRING_IMPLEMENTATION
#include "sta-main/sta_string.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//example a_cdh and a_sub( both can null)
void cdh_free(char * p)
{
	puts(p);
	free(p);
}
int cdh_foo(a_cdh)
{
	puts("in cdh foo");
	char * v = a_string_copy_malloc("a_string_copy_malloc");
a_handle(v,cdh_free)

	//use a_cdh
}
#define foo(expr) cdh_foo(a_sub) 

//example a_begin and a_return
int fun(void)
{
	a_begin()
	
	puts("calling cdh_foo");
	cdh_foo(a_sub);
	puts("calling foo");
	foo();
	
	a_return(0)
}

int main(int argc, const char * argv[])
{
fun();
	return 0;
}

#define STA_STRING_IMPLEMENTATION
#include "..\..\..\sta_string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int func(void)
{
	a_begin()

	FILE * f = fopen("main.c","rb");

	a_handle(f,fclose)

    // a_check_error //cdh->is_error
	//a_ok(p)
  //  a_end // without using a_return or return_ok 
  
	a_return(0)
}


int main(int argc, const char * argv[])
{
	a_begin()
  const char * hello = "hello world";
  char * msg = ssz_1(hello);
  a_handle(msg,free)
	 
	puts(msg);
  msg = ssz_file("main.c");
  puts(msg);

	a_return(0);
}
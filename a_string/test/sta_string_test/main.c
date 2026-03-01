
#define STA_STRING_IMPLEMENTATION
#include "..\..\..\sta_string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
  const char * hello = "hello world";
  char * msg = (char*)a_string_copy_malloc(hello);
  if(msg)
  {
	 
	puts(msg);
	
	free(msg);
  }
	return 0;
}
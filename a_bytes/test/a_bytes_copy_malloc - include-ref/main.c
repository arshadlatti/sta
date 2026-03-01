
//#define STA_BYTES_IMPLEMENTATION
#include "..\..\a_bytes.h"

#include "auto/auto_inc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
  const char * hello = "hello world";
  char * msg = (char*)a_bytes_copy_malloc(hello,strlen(hello)+1);
  if(msg)
  {
	 
	puts(msg);
	
	free(msg);
  }
	return 0;
}


#include "..\..\a_bits.h"

#include "auto/auto_inc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, const char * argv[])
{
	 uint8_t b=0;
int i;
do
{
	i=b;
	printf("%i\n",i);
}while(a_inc_bits(&b,8)); 
	return 0;
}
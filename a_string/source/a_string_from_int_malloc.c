
char * a_string_from_int_malloc(int v)
{
	// sprintf(str,"%d",value) converts to decimal base.
// sprintf(str,"%x",value) converts to hexadecimal base.
// sprintf(str,"%o",value) converts to octal base.

	char str[80];
	//itoa(v,str,10);
	sprintf(str,"%d",v);
	return a_string_copy_malloc(str);
}
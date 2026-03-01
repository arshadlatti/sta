
char * a_string_from_char_array_malloc(const char * ca,int cc)
{
	char * r = (char*)malloc(cc+1);
	if(r)
	{
		memcpy(r,ca,cc);
	r[cc]=0;
	}
 return r;
}

wchar_t * w_string_from_a_string_malloc(const char * s)
{
	int n = strlen(s);
	char * c;
	//n++;
	uint16_t * r = (uint16_t*) malloc((n+1)*2);
	if(r)
	{
		r[n]=0;
	//int i;
	for_i(n)
	{
		r[i]=0;//for zero upper byte
		c=(char*)&r[i];
		*c=s[i];
	}
	
	}
	return (wchar_t*)r;
	
}
// cc  = 3 s = "1" c = '0' return value ="001"
char * a_string_copy_fixed_size_malloc(const char * s,const char c,int cc)
{
	int l = strlen(s);
	int missed = cc - l;
	char * r =(char*)malloc(cc+l+1);//cc to missed
	char * p = r;
	if(r)
	{
		for_i(missed)
		{
			p[0] = c;
			p++;
		}
		strcpy(p,s);
	}
	
	return r;
}
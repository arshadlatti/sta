void a_string_replace_chars(char * s,const char * c,char new_c)
{
	const char * f;
	while(*s)
	{
		f=c;
		while(*f)
		{
		if(*s == *f) *s = new_c;
		f++;
		}
		s++;
	}
}
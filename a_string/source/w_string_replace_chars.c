
void w_string_replace_chars(wchar_t * s,const wchar_t * c,wchar_t new_c)
{
	const wchar_t * f;
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
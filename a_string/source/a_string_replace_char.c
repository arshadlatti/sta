
void a_string_replace_char(char * s,char c,char new_c)
{
	while(*s)
	{
		
		if(*s == c) *s = new_c;
		s++;
	}
}

char * a_string_copy_stop_at_char_malloc(const char * s,int c)
{
	char * path_e = a_string_copy_malloc(s);
	char * r = 0;
	char * p;
	if(path_e)
	{
		p = path_e;
		while(*p)
		{
			if(p[0] == c)
			{
				p[0]=0;
				break;
			}
			p++;
		}
		r = a_string_copy_malloc(path_e);
		free(path_e);
	}
	return r;
}

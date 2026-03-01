char * a_string_get_between_malloc(const char * s,const char * after,const char * stop_on)
{
	char * r = a_string_get_after_malloc(s,after);
	if(r)
	{
		
		int i = a_index_of_string(r,stop_on);
		if(i  >= 0)
		{
			r[i]=0;
			return r;
		}
		
		free(r);
		return a_null;
	}

	return r;
}
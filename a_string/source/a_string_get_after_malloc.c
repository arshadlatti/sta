char * a_string_get_after_malloc(const char * s,const char * ss)
{
	char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return a_string_copy_malloc(&s[i+strlen(ss)]);
	}
	return r;
}
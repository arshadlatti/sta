
char * a_string_get_after_ptr(const char * s,const char * ss)
{
	//char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return (char*)&s[i+strlen(ss)];
	}
	return a_null;
}

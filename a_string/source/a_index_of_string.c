int a_index_of_string(const char * s,const char * ss)
{
	char * p = strstr(s,ss);
	if(p)
	{
		return p - s;
	}
	
	return -1;
}


a_bool a_string_is_valid(const char * s)
{
	if(s)
	{
		if(*s)
			return a_true;
	}
	return a_false;
}
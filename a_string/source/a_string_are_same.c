
a_bool a_string_are_same(const char * a,const char * b)
{
	int cca = strlen(a);
	int ccb = strlen(b);
	if(cca == ccb)
	{
		for_i(cca)
		{
			if(a[i] != b[i])
				return a_false;
		}
		return a_true;
	}
	return a_false;
}

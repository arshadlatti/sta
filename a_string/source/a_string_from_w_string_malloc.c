

char * a_string_from_w_string_malloc(const wchar_t * wc_str)
{
	int n = wcslen(wc_str);
	//char * c;
	//n++;
	char * r = (char*) malloc((n+1));
	
if(r)
{
	r[n]=0;
	for_i(n)
	{
		r[i]=wc_str[i];
	}
}
	return r;
}

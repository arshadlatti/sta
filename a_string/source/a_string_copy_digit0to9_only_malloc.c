
char * a_string_copy_digit0to9_only_malloc(const char * s)
{
	
	int cc = strlen(s);
	 char * r=(char * )malloc(cc+1);
	 int dd=0;
	if(r)
	{
		//puts_s_i("count",cc);
		for_i(cc)
		{
		//	putch(s[i]);
			if(s[i] >='0' && s[i] <='9')
			{
				
				r[dd]=s[i];
				dd++;
			}
		}
		r[dd]=0;
		
	}
	return r;
}
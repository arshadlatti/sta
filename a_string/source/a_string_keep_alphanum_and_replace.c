
void a_string_keep_alphanum_and_replace(char * s,const char * keep,int c_any_other)
{
			int cc = strlen(s);
		for_i(cc)
		{
			if(!isalnum(s[i]) && a_index_of_char(keep,s[i]) < 0 )
				s[i] = c_any_other;
		}
}


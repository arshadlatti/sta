

char * a_string_copy_2_malloc(const char * s1,const char * s2)
{
		char * r = (char*) malloc(strlen(s1)+strlen(s2)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		}
		return r;		
}


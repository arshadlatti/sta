char * a_string_copy_3_malloc(const char * s1,const char * s2,const char * s3)
{
			char * r = (char*) malloc(strlen(s1)+strlen(s2)+strlen(s3)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		strcat(r,s3);
		}
		return r;		
}

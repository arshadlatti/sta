void a_bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
	char * p = (char*) bytes_src;
	p+=start;
	if(len)
		memcpy(bytes_dst,p,len);
}
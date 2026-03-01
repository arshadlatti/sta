void a_bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
		char * p = (char*) bytes_dst;
	p+=start;
	if(len)
		memcpy(p,bytes_src,len);
}
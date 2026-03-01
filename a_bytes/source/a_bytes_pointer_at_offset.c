void * a_bytes_pointer_at_offset(void * base,int offset)
{
	char * p = (char*)base;
	p+=offset;
	return p;
}
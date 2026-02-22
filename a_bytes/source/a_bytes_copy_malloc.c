
void * a_bytes_copy_malloc(const void * bytes,int bytes_size)
{
	if(!bytes_size) return 0;
	void * r = malloc(bytes_size);
	if(r)
	 memcpy(r,bytes,bytes_size);
return r;
}

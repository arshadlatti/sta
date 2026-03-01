
void * sta_malloc_ex(void ** out_ptr_ptr,size_t size)
{
	void * mem = malloc(size);
	if(out_ptr_ptr)
		*out_ptr_ptr = mem;
	
	return mem;
}
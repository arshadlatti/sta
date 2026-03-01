void * a_bytes_malloc(void ** out_ptr_ptr,size_t size)
{
	*out_ptr_ptr = malloc(size);
	return *out_ptr_ptr;
}
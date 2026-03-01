
void * a_bytes_from_file_malloc(const char * file_name,long * out_size)
{
	return a_bytes_can_null_term_from_file_malloc(file_name,a_false,out_size);
}


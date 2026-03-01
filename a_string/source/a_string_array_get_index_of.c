// >= 0 mean valid and on -1 mean not found.
a_bool a_string_array_get_index_of(const char ** array,int array_size,const char * s_to_be_found)
{
	for_i(array_size)
	{
	     if(a_string_are_same(array[i],s_to_be_found)) return i;
	}
	
	return -1;//a_not_found
}
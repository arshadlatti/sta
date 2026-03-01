
 char * a_string_from_file_malloc(const char * file_name)
{
	return (char * ) a_bytes_can_null_term_from_file_malloc(file_name,a_true,a_null);
}
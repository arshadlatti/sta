char * a_string_copy_malloc(const char * s)
{
	return a_string_from_char_array_malloc(s,strlen(s));
}

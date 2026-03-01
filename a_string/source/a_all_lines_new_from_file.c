a_all_lines_t * a_all_lines_new_from_file(const char * filename)
{
	char * text_all = a_string_from_file_malloc(filename);
 

if(text_all)
	return a_all_lines_new_from_string(text_all,a_false,a_true);

return a_null;
}





a_bool a_string_to_file(const char * file_name,const char * text)
{
	return a_bytes_to_file(file_name,text,strlen(text));
}


a_bool a_all_lines_next_peek(a_all_lines_t * al,char ** line,int * line_size)
{
	char * lines = al->lines;
	return a_text_read_line(&lines,line,line_size);
}


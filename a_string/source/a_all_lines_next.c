

a_bool a_all_lines_next(a_all_lines_t * al)
{
	a_bool r;
	r = a_text_read_line(&al->lines,&al->line,&al->line_size);
	if(r) al->line_count++;
	return r;
}



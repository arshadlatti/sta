int a_all_lines_count(a_all_lines_t * al)
{
	a_all_lines_t a;
	int r=0;
	a.lines = al->text_all;
	a.line_count = 0;
	while(a_all_lines_next(&a)) r++;
	return r;
}

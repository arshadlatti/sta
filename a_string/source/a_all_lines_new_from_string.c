
a_all_lines_t * a_all_lines_new_from_string( char * s,a_bool do_malloc,a_bool is_malloc)
{
	a_all_lines_t * r = malloc(sizeof(a_all_lines_t));

	if(r)
	{
		if(do_malloc)
		{
		r->text_all= a_string_copy_malloc(s);
		if(!r->text_all)
		{
			free(r);
			return a_null;
		}
		r->text_all_is_malloc = a_true;
		r->lines = r->text_all;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		else
		{
		r->text_all= s;
		r->text_all_is_malloc = is_malloc;
		r->lines = s;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		
		
	}
	return r;
}
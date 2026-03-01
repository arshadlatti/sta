void a_all_lines_delete(a_all_lines_t * al)
{
	if(al->text_all_is_malloc)
		free(al->text_all);
	free(al);
}

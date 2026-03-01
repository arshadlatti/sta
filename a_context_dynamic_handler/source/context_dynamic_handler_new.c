context_dynamic_handler_t * context_dynamic_handler_new(void)
{
	context_dynamic_handler_t * r = (context_dynamic_handler_t *) malloc(sizeof(context_dynamic_handler_t));
	if(r)
	{
		
	
	r->dh = gt_list_new_ex(sizeof(context_dynamic_handler__item_t),a_null,context_dynamic_handler__item_term);
	if(!r->dh)
	{
		free(r);
		return a_null;
	}
	r->is_error=0;
	r->r=a_null;
	r->sub= a_null;
	
	}
	
	return r;
}
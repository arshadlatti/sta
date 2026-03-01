
void context_dynamic_handler__item_term(void * p)
{
	
	context_dynamic_handler__item_t * itm=(context_dynamic_handler__item_t*)p;
	void ** ptr;
	if(!itm)
		return;
	
	if(!itm->term_func)
		return;
	
	if(!itm->ptr)
		return;
	
	if(itm->is_var)
	{
		ptr = (void**)itm->ptr;
		if(*ptr)(*itm->term_func)(*ptr); //	(*itm->term_func)(*ptr);
	}
	else
	{
		(*itm->term_func)(itm->ptr);
	}
	
}
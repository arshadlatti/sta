void * context_dynamic_handler_add(context_dynamic_handler_t * cdh,void * ptr,void * func)
{
	
		if(!cdh) return a_null;
	
	
	context_dynamic_handler__item_t  item;
	item.ptr = ptr;
	item.is_var = a_false;
	item.term_func = (gt_term_free_func)func;
	if(gt_list_add(cdh->dh,&item))
		return ptr;
 return a_null;
}

void * context_dynamic_handler_add_var(context_dynamic_handler_t * cdh,void ** var_ptr,void * func){
	
	if(!cdh) return a_null;
	
		context_dynamic_handler__item_t item;
	item.ptr = (void*)var_ptr;
	item.is_var = a_true;
	item.term_func = (gt_term_free_func)func;
	if(gt_list_add(cdh->dh,&item))
		return (void*)var_ptr;
 return a_null;
}

void context_dynamic_handler_delete(context_dynamic_handler_t * cdh)
{
	if(cdh)
	{
	 if(cdh->sub)
       context_dynamic_handler_delete(cdh->sub);
      gt_list_delete(cdh->dh);
	  free(cdh);
	}
}

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


void * context_dynamic_handler_set_error(context_dynamic_handler_t * cdh,int is_error)
{
	
	if(cdh)
	{
		cdh->is_error = is_error;
	}
}

context_dynamic_handler_t * context_dynamic_handler_sub(context_dynamic_handler_t * cdh)
{
	if(cdh)
	{
		if(cdh->sub)
			context_dynamic_handler_delete(cdh->sub);
		cdh->sub = context_dynamic_handler_new();
		return cdh->sub;
	}
	return a_null;
}


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


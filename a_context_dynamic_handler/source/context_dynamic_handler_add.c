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
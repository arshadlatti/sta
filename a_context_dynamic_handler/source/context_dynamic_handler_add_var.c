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
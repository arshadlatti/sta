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
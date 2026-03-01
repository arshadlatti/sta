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
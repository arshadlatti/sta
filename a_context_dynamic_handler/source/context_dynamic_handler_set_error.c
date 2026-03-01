
void * context_dynamic_handler_set_error(context_dynamic_handler_t * cdh,int is_error)
{
	
	if(cdh)
	{
		cdh->is_error = is_error;
	}
}
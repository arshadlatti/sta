wchar_t  uuz__empty[4]={0};
wchar_t * uuz__cdh(a_cdh,wchar_t * s)
{
	
		if(s)
	{
		wchar_t * r = (wchar_t*)context_dynamic_handler_add(cdh,s,(void*)free);
	if(!r) 
	{
		context_dynamic_handler_set_error(cdh,a_true);
		free(s);
	}
	else 
		return r;
	}
	else 
	{
		context_dynamic_handler_set_error(cdh,a_true);
	}
	
	return uuz__empty;
}



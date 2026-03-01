

char  ssz__empty[4]={0};
//s from malloc
char * ssz__cdh(a_cdh,char * s)
{
	if(s)
	{
		char * r = (char*)context_dynamic_handler_add(cdh,s,(void*)free);
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
	
	return ssz__empty;
}

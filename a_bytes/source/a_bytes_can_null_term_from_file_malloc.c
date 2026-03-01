
 void * a_bytes_can_null_term_from_file_malloc(const char * file_name,a_bool be_null_term_output,long * out_size)
{

	FILE * f = fopen(file_name,"rb");
	if(!f)
		return a_null;
		
	if(fseek(f,0,SEEK_END))
	{
		fclose(f);
		return a_null;
	}

	long size = ftell(f);
	
	void* buffer=a_null;
	char * text;

	
    if(size > 0)
	{
		if(be_null_term_output)
		size++;
	
		if(out_size)
	*out_size = size;
	
		if(!fseek(f,0,SEEK_SET))
		{
			
			buffer = malloc(size);
			if(buffer)
			{
				if(!fread(buffer,1,size,f))
				{
					free(buffer);
					fclose(f);
					return a_null;
				}
				if(be_null_term_output)
				{
					text = (char*)buffer;
					text[size - 1]=0;
				}
				
			}
		}
		
	}		
	fclose(f);	
	return buffer;
}
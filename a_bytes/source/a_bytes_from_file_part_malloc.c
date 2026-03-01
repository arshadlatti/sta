

void * a_bytes_from_file_part_malloc(const char * file_name,int offset,int size)
{
		FILE * f = fopen(file_name,"rb");
	if(!f)
		return a_null;
		
	if(fseek(f,0,SEEK_END))
	{
		fclose(f);
		return a_null;
	}

	long file_size = ftell(f);
	
	
	void* buffer=a_null;
	char * text;

	
    if(file_size >= size + offset)
	{
		/* if(be_null_term_output)
		size++; */
	
		if(!fseek(f,offset,SEEK_SET))
		{
			
			buffer = malloc(size);
			if(buffer)
			{
				if(fread(buffer,1,size,f) != size)
				{
					free(buffer);
					fclose(f);
					return a_null;
				}
		
				
			}
		}
		
	}		
	fclose(f);

return buffer;	
}
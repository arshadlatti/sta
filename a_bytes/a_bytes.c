
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


void * a_bytes_copy_2_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2)
{
	if(!bytes_size_1 && !bytes_size_2) return 0;
	char * r = (char*)malloc(bytes_size_1 + bytes_size_2);
	if(r)
	{
		if(bytes_size_1)
			memcpy(r,bytes_1,bytes_size_1);
		if(bytes_size_2)
			memcpy(&r[bytes_size_1],bytes_2,bytes_size_2);
	}
	return r;
}


void * a_bytes_copy_3_malloc(const void * bytes_1,int bytes_size_1,const void * bytes_2,int bytes_size_2,const void * bytes_3,int bytes_size_3)
{
		if(!bytes_size_1 && !bytes_size_2) return 0;
	char * r = (char*)malloc(bytes_size_1 + bytes_size_2 + bytes_size_3);
	if(r)
	{
		if(bytes_size_1)
			memcpy(r,bytes_1,bytes_size_1);
		if(bytes_size_2)
			memcpy(&r[bytes_size_1],bytes_2,bytes_size_2);
		if(bytes_size_3)
			memcpy(&r[bytes_size_1 + bytes_size_2],bytes_3,bytes_size_3);
	}
	return r;
}


void * a_bytes_copy_malloc(const void * bytes,int bytes_size)
{
	if(!bytes_size) return 0;
	void * r = malloc(bytes_size);
	if(r)
	 memcpy(r,bytes,bytes_size);
return r;
}


void * a_bytes_from_file_malloc(const char * file_name,long * out_size)
{
	return a_bytes_can_null_term_from_file_malloc(file_name,a_false,out_size);
}




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

void * a_bytes_malloc(void ** out_ptr_ptr,size_t size)
{
	*out_ptr_ptr = malloc(size);
	return *out_ptr_ptr;
}

void * a_bytes_pointer_at_offset(void * base,int offset)
{
	char * p = (char*)base;
	p+=offset;
	return p;
}

void a_bytes_read_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
	char * p = (char*) bytes_src;
	p+=start;
	if(len)
		memcpy(bytes_dst,p,len);
}


a_bool a_bytes_to_file(const char * file_name,const void * buffer,long buffer_size)
{
	FILE * f=fopen(file_name,"wb");
	if(f)
	{
		if(fwrite(buffer,1,buffer_size,f) == buffer_size)
		{
		fclose(f);
		return a_true;
		}
		fclose(f);
	}
	
	return a_false;
}


void a_bytes_write_at(void * bytes_dst,const void * bytes_src,int start,int len)
{
		char * p = (char*) bytes_dst;
	p+=start;
	if(len)
		memcpy(p,bytes_src,len);
}


 char * a_string_from_file_malloc(const char * file_name)
{
	return (char * ) a_bytes_can_null_term_from_file_malloc(file_name,a_true,a_null);
}






a_bool a_string_to_file(const char * file_name,const char * text)
{
	return a_bytes_to_file(file_name,text,strlen(text));
}


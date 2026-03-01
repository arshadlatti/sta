
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


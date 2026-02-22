
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

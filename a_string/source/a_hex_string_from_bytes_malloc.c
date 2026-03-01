
char * a_hex_string_from_bytes_malloc(const uint8_t * mem,int mem_size)
{
	const char hx[]="0123456789ABCDEF";
	char * r = (char * ) malloc(mem_size*2+1);
	if(r)
	{
		for_i(mem_size)
		{
			r[i*2] = hx[mem[i] / 16];
			r[i*2+1] = hx[mem[i] % 16];
		}
		r[mem_size*2] = 0;
	}
	
	return r;
}
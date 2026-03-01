
uint8_t* a_bytes_from_hex_string_malloc(const char * hs,int extra_bytes,int * out_size)
{
	int binary_size = strlen(hs) / 2 ;
	if(!binary_size) return 0;
	int bytes_count = binary_size+ extra_bytes;
	
	uint8_t* r = (uint8_t*)malloc(bytes_count);
	
	if(r)
	{
		if(out_size) *out_size = bytes_count;
		
		memset(r,0,bytes_count);
		
		uint8_t bin;
		for_i(binary_size)
		{
			
			bin =a_char_hex_to_int(hs[i*2]) * 16;
			bin += a_char_hex_to_int(hs[(i*2)+1]);
			r[i] = bin;
		}
	}
		
	
	return r;
}
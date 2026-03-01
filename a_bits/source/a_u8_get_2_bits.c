
int a_u8_get_2_bits(uint8_t b,int bit_num)
{
			int index = 0;
		if(a_u8_get_bit(b,bit_num))
           index = 2;
	   
	   if(a_u8_get_bit(b,bit_num+1))
           index += 1;
	   
	   return index;
}

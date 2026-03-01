a_bool a_u8_get_bit(uint8_t b,int bit_num)
{
if( b & (1<<bit_num))
	return a_true;
return a_false;
}
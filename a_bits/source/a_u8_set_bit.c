void a_u8_set_bit(uint8_t * b,int bit_num,a_bool v)
{
		if(v)
		b[0] |= 1 << bit_num;
	else
		b[0] &= ~(1 << bit_num);
}

void a_set_2_bits(uint8_t * bits,int loc,a_bool v,a_bool v2)
{
	a_set_bit(bits,loc,v);
		a_set_bit(bits,loc+1,v2);
}
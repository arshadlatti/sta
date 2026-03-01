
a_bool a_inc_bits_r(uint8_t * bits,int max_bits)
{
	
	if(!max_bits) return a_false;
	
	int i;
	for(i=max_bits-1;i>=0;i--)
	{
		if(a_get_bit(bits,i))
		{
			a_set_bit(bits,i,0);
		}
		else
		{
			a_set_bit(bits,i,1);
			return a_true;
		}
	}
	
	return a_false;
}

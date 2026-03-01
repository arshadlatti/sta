

a_bool a_inc_bits(uint8_t * bits,int max_bits)
{
	int i;
	for(i=0;i<max_bits;i++)
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

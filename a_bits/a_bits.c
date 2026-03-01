
a_bool a_get_bit(const uint8_t * bits,int loc)
{
	return a_u8_get_bit(bits[loc/8],loc%8);
}



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


void a_set_2_bits(uint8_t * bits,int loc,a_bool v,a_bool v2)
{
	a_set_bit(bits,loc,v);
		a_set_bit(bits,loc+1,v2);
}


void a_set_bit(uint8_t * bits,int loc,int v)
{
	a_u8_set_bit(&bits[loc/8],loc%8,v);
}


int a_u8_get_2_bits(uint8_t b,int bit_num)
{
			int index = 0;
		if(a_u8_get_bit(b,bit_num))
           index = 2;
	   
	   if(a_u8_get_bit(b,bit_num+1))
           index += 1;
	   
	   return index;
}

a_bool a_u8_get_bit(uint8_t b,int bit_num)
{
if( b & (1<<bit_num))
	return a_true;
return a_false;
}

void a_u8_set_bit(uint8_t * b,int bit_num,a_bool v)
{
		if(v)
		b[0] |= 1 << bit_num;
	else
		b[0] &= ~(1 << bit_num);
}



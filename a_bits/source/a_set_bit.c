
void a_set_bit(uint8_t * bits,int loc,int v)
{
	a_u8_set_bit(&bits[loc/8],loc%8,v);
}

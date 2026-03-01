a_bool a_get_bit(const uint8_t * bits,int loc)
{
	return a_u8_get_bit(bits[loc/8],loc%8);
}

// 0..9 and A..F  to 0..15
int a_char_hex_to_int(int c)
{
	if(c >= '0' && c <= '9')
		return c - '0';
	if(c >= 'A' && c <= 'F')
		return (c - 'A') + 10;
	
	if(c >= 'a' && c <= 'f')
		return (c - 'a') + 10;
	return 0;
}
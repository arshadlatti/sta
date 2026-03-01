int a_index_of_char(const char * s,int c)
{
	for_i_cc(s){
		if(s[i]== c) return i;
	}
	
	return -1;
}

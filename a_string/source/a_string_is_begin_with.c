

a_bool a_string_is_begin_with(const char * s, const char * s_begin)
{

	if(strstr(s,s_begin) == s) return a_true;

return a_false;
}
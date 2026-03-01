a_bool a_string_is_end_with(const char * s, const char * s_end)
{
	int cc = strlen(s);
	int cc_end = strlen(s_end);
if(cc >= cc_end)
{
	const char * begin = &s[cc - cc_end];
	if(strstr(begin,s_end) == begin) return a_true;
}
return a_false;
}
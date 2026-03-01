

char * a_url_from_http_HEAD_header_malloc( char * http_header)
{
	const char * uri_begin;
	char * uri_end;
	char * url=a_null;
	
	uri_begin=strstr(http_header,"HEAD ");
	if(uri_begin && uri_begin == http_header)
	{
		uri_begin+=5;
		if(uri_end=strstr(uri_begin," "))
		{
			int len = uri_end - uri_begin;
			if(len >0);
			url = a_string_from_char_array_malloc(uri_begin,len);
		}
	}
	return url;
}

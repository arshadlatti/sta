int a_all_lines_count(a_all_lines_t * al)
{
	a_all_lines_t a;
	int r=0;
	a.lines = al->text_all;
	a.line_count = 0;
	while(a_all_lines_next(&a)) r++;
	return r;
}

void a_all_lines_delete(a_all_lines_t * al)
{
	if(al->text_all_is_malloc)
		free(al->text_all);
	free(al);
}

a_all_lines_t * a_all_lines_new_from_file(const char * filename)
{
	char * text_all = a_string_from_file_malloc(filename);
 

if(text_all)
	return a_all_lines_new_from_string(text_all,a_false,a_true);

return a_null;
}


a_all_lines_t * a_all_lines_new_from_string( char * s,a_bool do_malloc,a_bool is_malloc)
{
	a_all_lines_t * r = malloc(sizeof(a_all_lines_t));

	if(r)
	{
		if(do_malloc)
		{
		r->text_all= a_string_copy_malloc(s);
		if(!r->text_all)
		{
			free(r);
			return a_null;
		}
		r->text_all_is_malloc = a_true;
		r->lines = r->text_all;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		else
		{
		r->text_all= s;
		r->text_all_is_malloc = is_malloc;
		r->lines = s;
	/* 	r->line = a_null;
		r->line_size = 0; */
		r->line_count = 0;
		}
		
		
	}
	return r;
}



a_bool a_all_lines_next(a_all_lines_t * al)
{
	a_bool r;
	r = a_text_read_line(&al->lines,&al->line,&al->line_size);
	if(r) al->line_count++;
	return r;
}




a_bool a_all_lines_next_peek(a_all_lines_t * al,char ** line,int * line_size)
{
	char * lines = al->lines;
	return a_text_read_line(&lines,line,line_size);
}


a_bool a_all_lines_reset(a_all_lines_t * al)
{
		al->lines = al->text_all;
		al->line_count = 0;
}



uint8_t* a_bytes_from_hex_string_malloc(const char * hs,int extra_bytes,int * out_size)
{
	int binary_size = strlen(hs) / 2 ;
	if(!binary_size) return 0;
	int bytes_count = binary_size+ extra_bytes;
	
	uint8_t* r = (uint8_t*)malloc(bytes_count);
	
	if(r)
	{
		if(out_size) *out_size = bytes_count;
		
		memset(r,0,bytes_count);
		
		uint8_t bin;
		for_i(binary_size)
		{
			
			bin =a_char_hex_to_int(hs[i*2]) * 16;
			bin += a_char_hex_to_int(hs[(i*2)+1]);
			r[i] = bin;
		}
	}
		
	
	return r;
}


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


char * a_hex_string_from_bytes_malloc(const uint8_t * mem,int mem_size)
{
	const char hx[]="0123456789ABCDEF";
	char * r = (char * ) malloc(mem_size*2+1);
	if(r)
	{
		for_i(mem_size)
		{
			r[i*2] = hx[mem[i] / 16];
			r[i*2+1] = hx[mem[i] % 16];
		}
		r[mem_size*2] = 0;
	}
	
	return r;
}

int a_index_of_char(const char * s,int c)
{
	for_i_cc(s){
		if(s[i]== c) return i;
	}
	
	return -1;
}

int a_index_of_string(const char * s,const char * ss)
{
	char * p = strstr(s,ss);
	if(p)
	{
		return p - s;
	}
	
	return -1;
}


a_bool a_string_are_same(const char * a,const char * b)
{
	int cca = strlen(a);
	int ccb = strlen(b);
	if(cca == ccb)
	{
		for_i(cca)
		{
			if(a[i] != b[i])
				return a_false;
		}
		return a_true;
	}
	return a_false;
}

// >= 0 mean valid and on -1 mean not found.
a_bool a_string_array_get_index_of(const char ** array,int array_size,const char * s_to_be_found)
{
	for_i(array_size)
	{
	     if(a_string_are_same(array[i],s_to_be_found)) return i;
	}
	
	return -1;//a_not_found
}



char * a_string_copy_2_malloc(const char * s1,const char * s2)
{
		char * r = (char*) malloc(strlen(s1)+strlen(s2)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		}
		return r;		
}


char * a_string_copy_3_malloc(const char * s1,const char * s2,const char * s3)
{
			char * r = (char*) malloc(strlen(s1)+strlen(s2)+strlen(s3)+1);
		if(r)
		{
		strcpy(r,s1);
		strcat(r,s2);
		strcat(r,s3);
		}
		return r;		
}


char * a_string_copy_digit0to9_only_malloc(const char * s)
{
	
	int cc = strlen(s);
	 char * r=(char * )malloc(cc+1);
	 int dd=0;
	if(r)
	{
		//puts_s_i("count",cc);
		for_i(cc)
		{
		//	putch(s[i]);
			if(s[i] >='0' && s[i] <='9')
			{
				
				r[dd]=s[i];
				dd++;
			}
		}
		r[dd]=0;
		
	}
	return r;
}

// cc  = 3 s = "1" c = '0' return value ="001"
char * a_string_copy_fixed_size_malloc(const char * s,const char c,int cc)
{
	int l = strlen(s);
	int missed = cc - l;
	char * r =(char*)malloc(cc+l+1);//cc to missed
	char * p = r;
	if(r)
	{
		for_i(missed)
		{
			p[0] = c;
			p++;
		}
		strcpy(p,s);
	}
	
	return r;
}

char * a_string_copy_malloc(const char * s)
{
	return a_string_from_char_array_malloc(s,strlen(s));
}


char * a_string_copy_stop_at_char_malloc(const char * s,int c)
{
	char * path_e = a_string_copy_malloc(s);
	char * r = 0;
	char * p;
	if(path_e)
	{
		p = path_e;
		while(*p)
		{
			if(p[0] == c)
			{
				p[0]=0;
				break;
			}
			p++;
		}
		r = a_string_copy_malloc(path_e);
		free(path_e);
	}
	return r;
}


char * a_string_from_char_array_malloc(const char * ca,int cc)
{
	char * r = (char*)malloc(cc+1);
	if(r)
	{
		memcpy(r,ca,cc);
	r[cc]=0;
	}
 return r;
}


char * a_string_from_int_malloc(int v)
{
	// sprintf(str,"%d",value) converts to decimal base.
// sprintf(str,"%x",value) converts to hexadecimal base.
// sprintf(str,"%o",value) converts to octal base.

	char str[80];
	//itoa(v,str,10);
	sprintf(str,"%d",v);
	return a_string_copy_malloc(str);
}



char * a_string_from_w_string_malloc(const wchar_t * wc_str)
{
	int n = wcslen(wc_str);
	//char * c;
	//n++;
	char * r = (char*) malloc((n+1));
	
if(r)
{
	r[n]=0;
	for_i(n)
	{
		r[i]=wc_str[i];
	}
}
	return r;
}

char * a_string_get_after_malloc(const char * s,const char * ss)
{
	char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return a_string_copy_malloc(&s[i+strlen(ss)]);
	}
	return r;
}


char * a_string_get_after_ptr(const char * s,const char * ss)
{
	//char * r = a_null;
	int i = a_index_of_string(s,ss);
	if(i >= 0)
	{
		int cc= strlen(&s[i+strlen(ss)]);
		if(cc)
			return (char*)&s[i+strlen(ss)];
	}
	return a_null;
}

char * a_string_get_between_malloc(const char * s,const char * after,const char * stop_on)
{
	char * r = a_string_get_after_malloc(s,after);
	if(r)
	{
		
		int i = a_index_of_string(r,stop_on);
		if(i  >= 0)
		{
			r[i]=0;
			return r;
		}
		
		free(r);
		return a_null;
	}

	return r;
}



a_bool a_string_is_begin_with(const char * s, const char * s_begin)
{

	if(strstr(s,s_begin) == s) return a_true;

return a_false;
}

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



a_bool a_string_is_valid(const char * s)
{
	if(s)
	{
		if(*s)
			return a_true;
	}
	return a_false;
}


void a_string_keep_alphanum_and_replace(char * s,const char * keep,int c_any_other)
{
			int cc = strlen(s);
		for_i(cc)
		{
			if(!isalnum(s[i]) && a_index_of_char(keep,s[i]) < 0 )
				s[i] = c_any_other;
		}
}



void a_string_replace_char(char * s,char c,char new_c)
{
	while(*s)
	{
		
		if(*s == c) *s = new_c;
		s++;
	}
}

void a_string_replace_chars(char * s,const char * c,char new_c)
{
	const char * f;
	while(*s)
	{
		f=c;
		while(*f)
		{
		if(*s == *f) *s = new_c;
		f++;
		}
		s++;
	}
}

a_bool a_text_read_line(char ** lines,char ** line,int * line_size)
{
	int cc=0;
	char * text=*lines;
	*line = text;
	if(text[0] ==0)
		return a_false;
	
	while(*text)
	{
		if(*text == '\n' || *text == '\r')
		{
			if(*text == '\r')
			{
				text++;
				continue;
			}
			
			text++;
			*lines = text;
			*line_size = cc;
			return a_true;
		}
		cc++;
		text++;
		if(*text == 0)
		{
			*lines = text;
			*line_size = cc;
			return a_true;
		}
	}
	
	
	return a_false;
}



char * a_url_from_http_GET_header_malloc( char * http_header)
{
	const char * uri_begin;
	char * uri_end;
	char * url=a_null;
	uri_begin=strstr(http_header,"GET ");
	if(uri_begin && uri_begin == http_header)
	{
		uri_begin+=4;
		if(uri_end=strstr(uri_begin," "))
		{
			int len = uri_end - uri_begin;
			if(len >0);
			url = a_string_from_char_array_malloc(uri_begin,len);
		}
	}
	return url;
}




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



char  ssz__empty[4]={0};
//s from malloc
char * ssz__cdh(a_cdh,char * s)
{
	if(s)
	{
		char * r = (char*)context_dynamic_handler_add(cdh,s,(void*)free);
	if(!r) 
	{
		context_dynamic_handler_set_error(cdh,a_true);
		free(s);
	}
	else 
		return r;
	}
	else 
	{
		context_dynamic_handler_set_error(cdh,a_true);
	}
	
	return ssz__empty;
}

wchar_t  uuz__empty[4]={0};
wchar_t * uuz__cdh(a_cdh,wchar_t * s)
{
	
		if(s)
	{
		wchar_t * r = (wchar_t*)context_dynamic_handler_add(cdh,s,(void*)free);
	if(!r) 
	{
		context_dynamic_handler_set_error(cdh,a_true);
		free(s);
	}
	else 
		return r;
	}
	else 
	{
		context_dynamic_handler_set_error(cdh,a_true);
	}
	
	return uuz__empty;
}




wchar_t * w_string_from_a_string_malloc(const char * s)
{
	int n = strlen(s);
	char * c;
	//n++;
	uint16_t * r = (uint16_t*) malloc((n+1)*2);
	if(r)
	{
		r[n]=0;
	//int i;
	for_i(n)
	{
		r[i]=0;//for zero upper byte
		c=(char*)&r[i];
		*c=s[i];
	}
	
	}
	return (wchar_t*)r;
	
}


void w_string_replace_chars(wchar_t * s,const wchar_t * c,wchar_t new_c)
{
	const wchar_t * f;
	while(*s)
	{
		f=c;
		while(*f)
		{
		if(*s == *f) *s = new_c;
		f++;
		}
		s++;
	}
}


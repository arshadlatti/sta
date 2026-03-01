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


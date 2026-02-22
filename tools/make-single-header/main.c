#include "project/project.h" 

int ts_add_file(text_stream_t * ts,const char * s,int cc)
{
	sub_begin()
	char * line_copy = string_copy_char_array_malloc(s,cc);
    handle_free(line_copy)
	char * filename = string_get_middle_malloc(line_copy,"[file:","]");
	
	handle_free(filename)
	puts(filename);
	for_lines_file(filename)
	if(lines->line_size)
	{
		ts_write_line_char_array(ts,lines->line,lines->line_size);
		//ts_write("\r\n");
	}
	else 
	{
		ts_write(ts,"\r\n");
	}
	next_lines
return_end(0);		
}


int main(int argc, const char * argv[])
{

argc_msg(3,"usage: make-single-header in.txt out.h");
sub_begin()
create_ts(ts)

for_lines_file(argv[1])
if(lines->line_size)
{
	if(lines->line[0] == '[')
	{
		if(lines->line_size > 1)
		{
			if(lines->line[1] == '[')
			{
				ts_write_line_char_array(ts,&lines->line[1],lines->line_size - 1);
				
			}
			else//check file  
			{
				ts_add_file(ts,lines->line,lines->line_size);
			}
		}
	}
	else
	{
		ts_write_line_char_array(ts,lines->line,lines->line_size);
	}
}
else
{
	ts_write(ts,"\r\n");
}
next_lines

if(ts->len)
	write_text_file(argv[2],ts->ptr);


	return_end(0);
}
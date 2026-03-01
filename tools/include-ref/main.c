#include "project/project.h" 


void free_free(char ** pp)
{
	if(*pp)
		free(*pp);
}

char ** ea_read_linker_file(const char * filename)
{
	char ** r;
	ea_blank(r,sizeof(char*),free_free);//all string added to array from malloc
	if(!r) return r;
	
	char * cur_line;
	char * cur_name;
	
	for_lines_file(filename)
	//lines->line char_array_ptr
	//lines->line_size char count 
	cur_line = string_from_char_array_malloc(lines->line,lines->line_size);
	if(cur_line)
	{
		cur_name = string_get_middle_malloc(cur_line,"`","'");
		if(cur_name)
		{
			
			
			if(string_array_is_already(r,ea_count(r),cur_name))
			{
			free(cur_name);
			}
			else
			{
				//puts(cur_name);
				ea_addv(r,cur_name);
			}
		}
		free(cur_line);
	}
	
	next_lines
	return r;
}

char * name2ref_malloc(const char * ref_filename,char ** name,int name_count)
{
	text_stream_t * ts = text_stream_new(0);
	if(!ts) return a_null;
	//ts_write_line(ts," ");
	
	char * cur_line;
	char * cur_source;
	int name_index;
	
	for_lines_file(ref_filename)
 
	cur_line = string_from_char_array_malloc(lines->line,lines->line_size);
	if(cur_line)
	{
		//puts(cur_line);
		name_index = string_index_of_char(cur_line,' ');
		if(name_index >= 0)
		{
			cur_line[name_index] =0;// now cur_line virtualy point to name
			cur_source=cur_line + name_index + 1;//cur_source point to source 
			if(string_array_is_already(name,name_count,cur_line))
			{
			//	puts(cur_source);
				ts_write_line(ts,cur_source);
			}
			
		}
		
		
		
		free(cur_line);
	}
	
	next_lines
	
	char * r=a_null;
	if(ts->len)
		r = string_copy_malloc(ts->ptr);
	text_stream_delete(ts);
	return r;
}

//char * multi_ref_file(text_stream_t )
int main(int argc, const char * argv[])
{
	argc_msg(4,"usage:include_ref.exe out.h in.txt ref.txt");
	char ** name = ea_read_linker_file(argv[2]);
	

	if(name)
	{
		//printf("reading ref...");
		int i;
		 char * out;
		 char * final;
		 char * pre=0;;
		for(i=3; i < argc;i++)
		{
		 out = name2ref_malloc(argv[i],name,ea_count(name));
	
		if(out)
		{
			if(pre)
				 final = string_append_malloc(pre,out);
			 else
				  final = string_append_malloc("\r\n",out);
			  
				 free(out);
				 if(final)
				 {
					 if(pre)
						 free(pre);
					 
					 pre = final;
				 }
		}
		
		}
	
		if(pre)
		{
		//	puts("[done]");
			write_text_file(argv[1],pre);
			free(pre);
		}
		
		ea_term(name); 
	}
	return 0;
}
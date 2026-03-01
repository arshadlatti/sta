#include "project/project.h" 
#define STA_WINDOWS_IMPLEMENTATION
#include "STA/Windows/STA_Windows.h"


int main(int argc, const char * argv[])
{
 argc_msg(2,"usage: get-auto-ref-maker a_lib_name");
 
 sub_begin()
 
 create_ts(ts_ref)
 create_ts(ts_maker)
 
 const char * lib_name = argv[1];
 char * function_name;
 for_win_find_files("*.c")
 
 function_name = string_stop_at_char_malloc(fd.cFileName,'.');
 if(function_name)
 {
	 puts(function_name);
	 
	 //[file:..\source\{filename}.c]
	 ts_write3(ts_maker,"[file:..\\source\\",fd.cFileName,"]\r\n\r\n");
	 
	 //{filename} space #include "sta-main\{a_lib_name}\source\{filename}.c"
	 ts_write3(ts_ref,function_name," #include \"sta-main\\",lib_name);
	 ts_write3(ts_ref,"\\source\\",fd.cFileName,"\"\r\n");

	 
	 free(function_name);
 }else {puts("error:");puts(fd.cFileName); }
 
 next_for_win_find_files
 
 



// ..\maker\{a_lib_name}_c.txt
 if(ts_maker->len)
 {
	 puts(ts_maker->ptr);
	 file_write_all_text(ssz_3("..\\maker\\",lib_name,"_c.txt"),ts_maker->ptr);
 }
	 
 
 
 //..\auto-ref\{a_lib_name}.txt 

 if(ts_ref->len)
 {
	 puts(ts_ref->ptr);
	 file_write_all_text(ssz_3("..\\auto-ref\\",lib_name,".txt"),ts_ref->ptr);
 }
 
 
	return_end(0);
}
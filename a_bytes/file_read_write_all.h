
// load_text(unicode) save_text(unicode utf8 utf16le/be)




#define create_read_file(data,data_size,file) 	long data_size;\
	uint8_t * data = (uint8_t*)file_binary_read_all_malloc(file,&data_size);\
	handle_free(data)

#define create_read_record_file(data,data_size_in_bytes,file,name_t) 	long data_size_in_bytes;\
	name_t * data = (name_t*)file_binary_read_all_malloc(file,&data_size_in_bytes);\
	if(data){if(data_size_in_bytes < sizeof(name_t)){ free(data); data = 0;}}\
	handle_free(data)
	
	
#define create_array_read_file(data,data_size,file,name_t) 	long data_size;\
	name_t * data = (name_t*)file_binary_read_all_malloc(file,&data_size);\
	handle_free(data)\
	data_size /= sizeof(name_t);
	








#ifdef STA_PROJECT_FINAL
//include all needed when project is completed so final size will become small

//#define STA_DYNAMIC_HANDLER_IMPLEMENTATION
//#include "STA/dynamic_handler.h"
#define STA_EXPANDABLE_ARRAY_IMPLEMENTATION
#include "STA/expandable_array.h"

#define STA_STRING_IMPLEMENTATION
#include "STA/sta_string.h"

#define BUFFER_STREM_IMPLEMENTATION
#include "STA/STD/buffer_stream.h"

#define ALL_LINES_IMPLEMENTATION
#include "STA/all_lines.h"

#define STA_TEXT_FILE_READER_IMPLEMENTATION
#include "STA/text_file_reader.h"



#define STA_FILE_READ_WRITE_ALL_IMPLEMENTATION
#include "STA\file_read_write_all.h"

#else


#define STA_ALL_IMPLEMENTATION
#include "STA/sta_all.h"

#endif

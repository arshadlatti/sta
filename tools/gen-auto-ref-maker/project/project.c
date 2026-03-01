


#ifdef STA_PROJECT_FINAL
//include all needed when project is completed so final size will become small

#define STA_DYNAMIC_HANDLER_IMPLEMENTATION
#include "STA/dynamic_handler.h"
#define BUFFER_STREM_IMPLEMENTATION
#include "STA/STD/buffer_stream.h"
#define STA_STRING_IMPLEMENTATION
#include "STA/sta_string.h"

#define STA_FILE_READ_WRITE_ALL_IMPLEMENTATION
#include "STA\file_read_write_all.h"
#define STA_STRING_SCOPED_IMPLEMENTATION
#include "STA/string_scoped.h"

#else


#define STA_ALL_IMPLEMENTATION
#include "STA/sta_all.h"

#endif

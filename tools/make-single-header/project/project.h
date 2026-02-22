#ifndef STA_PROJECT_H
#define STA_PROJECT_H

#include "STA/sta_def.h"

#ifdef STA_PROJECT_FINAL
//include all needed when project is completed so final size will become small

//#include "STA/dynamic_handler.h"

#include "STA/sta_all.h"
#else
#include "STA/sta_all.h"
#endif




#endif

/*
#define STA_PROJECT_IMPLEMENTATION
#include "STA/project.h"
*/


#ifdef STA_PROJECT_IMPLEMENTATION

#ifndef STA_PROJECT_IMPLEMENTATION_INCLUDED
#define STA_PROJECT_IMPLEMENTATION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "project.c"

#ifdef __cplusplus
}
#endif

#endif

#endif
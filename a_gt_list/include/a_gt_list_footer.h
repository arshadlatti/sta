
void * sta_malloc_ex(void ** out_ptr_ptr,size_t size);
#define sta_malloc(var,size) sta_malloc_ex((void**)var,size)

#ifdef __cplusplus
}
#endif

#endif


#ifndef STA_CONTEXT_DYNAMIC_HANDLER_H
#define STA_CONTEXT_DYNAMIC_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

struct s_context_dynamic_handler__item
{
	void * ptr;
	gt_term_free_func term_func;
	a_bool is_var;
};
typedef struct s_context_dynamic_handler__item context_dynamic_handler__item_t;
void context_dynamic_handler__item_term(void * p);

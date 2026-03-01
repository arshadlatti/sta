
//#define STA_GT_LIST_IMPLEMENTATION
//#include "sta-main/sta_gt_list.h"


struct s_gt_list__node
{
	struct s_gt_list__node * next;
	struct s_gt_list__node * pre;
	gt_term_free_func data_term_func;
	char data[sizeof(struct s_gt_list__node *)];
};
typedef struct s_gt_list__node  gt_list__node_t;

struct s_gt_list
{
	gt_list__node_t * node_first;
	gt_list__node_t * node_last;
	
	size_t item_size;
	size_t items_count;
	
	gt_init_with_func item_init_with_func;
	gt_term_free_func item_term_free_func;
};
typedef struct s_gt_list gt_list_t;



gt_list_t * gt_list_new_ex(size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
gt_list_t * gt_list_new(size_t item_size);
void gt_list_delete(gt_list_t * gtl);

a_bool gt_list_add_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);
a_bool gt_list_add(gt_list_t * gtl,void * item);

a_bool gt_list_add_first(gt_list_t * gtl,void * item);
a_bool gt_list_add_first_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func);


gt_list__node_t * gt_list_add_at(gt_list_t * gtl,void * item,int index);
gt_list__node_t * gt_list_add_at_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func,int index);


void * gt_list_get_at(gt_list_t * gtl,int index);
//a_bool gt_list_add_first_ex



gt_list__node_t * gt_list__node_add(gt_list_t * gtl,size_t item_size,a_bool is_at_first);
gt_list__node_t * gt_list__node_remove(gt_list_t * gtl,a_bool is_at_first);

gt_list__node_t * gt_list__node_add_at(gt_list_t * gtl,size_t item_size,int index);
gt_list__node_t * gt_list__node_get_at(gt_list_t * gtl,int index);


// A Project By Arshad Latti
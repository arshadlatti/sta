





a_bool gt_list_add_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list__node_t * node = gt_list__node_add(gtl,item_size,a_false);
	if(!node)
		return a_false;
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	
	return a_true;
	
}



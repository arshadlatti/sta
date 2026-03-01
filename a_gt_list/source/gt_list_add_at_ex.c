
gt_list__node_t * gt_list_add_at_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func,int index)
{
	gt_list__node_t * node = gt_list__node_add_at(gtl,item_size,index);
	if(node)
	{
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	}
	return node;
}

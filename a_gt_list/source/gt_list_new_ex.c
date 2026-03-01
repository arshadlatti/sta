gt_list_t * gt_list_new_ex(size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list_t * gtl;
	sta_malloc(&gtl,sizeof(gt_list_t));
	if(!gtl)
		return a_null;
	gtl->node_first = a_null;
	gtl->node_last  = a_null;
	gtl->item_size  = item_size;
	gtl->items_count=0;
	gtl->item_init_with_func = item_init_with_func;
	gtl->item_term_free_func = item_term_free_func;
	return gtl;//FIXED  not return gtl
}

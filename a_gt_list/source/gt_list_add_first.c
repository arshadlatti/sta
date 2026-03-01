

a_bool gt_list_add_first(gt_list_t * gtl,void * item)
{
	return gt_list_add_first_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func);
}
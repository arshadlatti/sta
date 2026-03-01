
void gt_list_delete(gt_list_t * gtl)
{
	gt_list__node_t * node;
	while((node = gt_list__node_remove(gtl,a_false)))
	{
		if(node->data_term_func)
			(*node->data_term_func)(node->data);
		free(node);
	}
	free(gtl);
}

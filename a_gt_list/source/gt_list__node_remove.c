

gt_list__node_t * gt_list__node_remove(gt_list_t * gtl,a_bool is_at_first)
{
	gt_list__node_t * node;
	
	if(!gtl->items_count)
		return a_null;
	
	if(gtl->items_count == 1)
	{
		node = gtl->node_first;
		gtl->items_count=0;
		return node;
	}
	
	gtl->items_count--;
	
	if(is_at_first)
	{
		node = gtl->node_first;
		gtl->node_first = node->next;
	}
	else
	{
		node = gtl->node_last;
		gtl->node_last = node->pre;
	}
	return node;
}


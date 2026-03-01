
gt_list__node_t * gt_list__node_add(gt_list_t * gtl,size_t item_size,a_bool is_at_first)
{
	gt_list__node_t * node;
	if(!sta_malloc(&node,sizeof(gt_list__node_t) + (item_size- sizeof(gt_list__node_t*))))
		return a_null;
	
	gtl->items_count++;
	if(gtl->items_count == 1)//if list is empty
	{
		gtl->node_first = node;
		gtl->node_last  = node;
		return node;
	}
	
	gt_list__node_t * node_pre;
	
	if(is_at_first)
	{
	node_pre = gtl->node_first;
	node_pre->pre = node;
	node->pre = a_null;
	node->next = node_pre;
	gtl->node_first = node;
	}
    else
	{
	node_pre = gtl->node_last;
	node_pre->next=node;
	node->pre = node_pre;
	node->next = a_null;
	gtl->node_last = node;
	}


	return node;
}




gt_list__node_t * gt_list__node_get_at(gt_list_t * gtl,int index)
{
		gt_list__node_t * node;
	if(!gtl->items_count)
		return a_null;
	if( index >= gtl->items_count)
		return a_null;
	int i = 0;
	node  = gtl->node_first;
	//if(index == 0)
		//return node;
	for(i=0;i< gtl->items_count;i++)
	{
			if(index == i)
			return node;
			node = node->next;
	if(!node)
		return a_null;
	}
	
	
	
	
	return a_null;
}
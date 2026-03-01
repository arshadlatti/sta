

gt_list__node_t * gt_list__node_add_at(gt_list_t * gtl,size_t item_size,int index)
{
	if(index == 0)
		return gt_list__node_add(gtl,item_size,a_true);
	if(index >= gtl->items_count)
		return gt_list__node_add(gtl,item_size,a_false);
	
	
	gt_list__node_t * node;
	
	   
	gt_list__node_t * node_pre;
    node_pre = gt_list__node_get_at(gtl,index);
	
	if(!node_pre)
		return a_null;
	
	if(!sta_malloc(&node,sizeof(gt_list__node_t) + (item_size- sizeof(gt_list__node_t*))))
		return a_null;
	
    node->pre = node_pre->pre;
	node->next = node_pre;
	
	node_pre->pre = node;
   node->pre->next = node;
	gtl->items_count++;

	return node;
	
}


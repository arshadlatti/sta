
void * gt_list_get_at(gt_list_t * gtl,int index)
{
	
	gt_list__node_t * node= gt_list__node_get_at(gtl,index);
	if(node)
	{
		
		return node->data;
	}
	
	return a_null;
}

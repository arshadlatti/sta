
a_bool gt_list_add(gt_list_t * gtl,void * item)
{
	return gt_list_add_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func);
}

gt_list__node_t * gt_list_add_at(gt_list_t * gtl,void * item,int index)
{
	
		return gt_list_add_at_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func,index);

	
}


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





a_bool gt_list_add_first(gt_list_t * gtl,void * item)
{
	return gt_list_add_first_ex(gtl,item,gtl->item_size,gtl->item_init_with_func,gtl->item_term_free_func);
}



a_bool gt_list_add_first_ex(gt_list_t * gtl,void * item,size_t item_size,gt_init_with_func item_init_with_func,gt_term_free_func item_term_free_func)
{
	gt_list__node_t * node = gt_list__node_add(gtl,item_size,a_true);
	if(!node)
		return a_false;
	
	if(item_init_with_func)
		(*item_init_with_func)(node->data,item);
	else
		memcpy(node->data,item,item_size);
	node->data_term_func = item_term_free_func;
	return a_true;
}



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


void * gt_list_get_at(gt_list_t * gtl,int index)
{
	
	gt_list__node_t * node= gt_list__node_get_at(gtl,index);
	if(node)
	{
		
		return node->data;
	}
	
	return a_null;
}


gt_list_t * gt_list_new(size_t item_size)
{
	return gt_list_new_ex(item_size,a_null,a_null);
}



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



void * sta_malloc_ex(void ** out_ptr_ptr,size_t size)
{
	void * mem = malloc(size);
	if(out_ptr_ptr)
		*out_ptr_ptr = mem;
	
	return mem;
}


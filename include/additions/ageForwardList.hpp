#pragma once



namespace age {

	template <class ItemM>
	class ForwardList;

	template <class ItemNode>
	class ForwardListNode {
		~ForwardListNode();
		ItemNode item_;
		ForwardListNode* next_node = 0;
		friend class ForwardList<ItemNode>;
	};

	template <class ItemM>
	class ForwardList {
		ForwardListNode<ItemM>* first_node;
		int childs_count = 0;
	public:
		~ForwardList();
		void add_item(ItemM item);
		ItemM* get_item(unsigned int id);
		int get_size() { return childs_count; }
	};


	
	template <class ItemNode>
	ForwardListNode<ItemNode>::~ForwardListNode()
	{
		if (next_node != 0)
			delete next_node;

	}

	template <class ItemM>
	ForwardList<ItemM>::~ForwardList()
	{
		delete first_node;
	}

	template <class ItemM>
	void ForwardList<ItemM>::add_item(ItemM item)
	{
		ForwardListNode<ItemM>** now_node = &first_node;
		while (*now_node != 0)
			now_node = &(*now_node)->next_node;
		*now_node = new ForwardListNode<ItemM>();
		(*now_node)->item_ = item;
		childs_count++;
	}

	template <class ItemM>
	ItemM* ForwardList<ItemM>::get_item(unsigned int id)
	{
		unsigned int i = 0;
		ForwardListNode<ItemM>* now_node = first_node;
		while (i < id) {
			now_node = now_node->next_node;
			i++;
		}
		return &now_node->item_;
	}
}

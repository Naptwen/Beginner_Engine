#pragma once
#include "Woo_linked_Block.h"

Block::Block()
{
	//printf(GREEN"| BLOCK | CREATE | %p " _RESET "\n", this);
}
Block::~Block()
{
	//printf(GREEN"| BLOCK | DELETE |  %p" _RESET "\n", this);
}
Blocklist::Blocklist()
{
	//printf(GREEN"| BLOCK LIST | CREATE | %p " _RESET "\n", this);
}
Blocklist::~Blocklist()
{
	//printf(GREEN"| BLOCK LIST | DELETE | " _RESET "\n", this);
	//printf(GREEN"| BLOCK LIST | THIS   | %p " _RESET "\n", this);
	void* next = this->address;
	for (int i = 0; i < this->list_size; i++)
	{
		//printf(GREEN"| BLOCK LIST | DELATE BLOCK | %p " _RESET "\n", next);
		Block* start = ((Block*)next);
		next = start->next;
		woolloc_free(start);
	}
}
//LINKED BLOCK SEARCHING
void* SEARCH(Blocklist *list, int number)
{
	void* nextaddr = list->address;
	int i = 0;
	while (true)
	{
		if (i == number)
		{
			break;
		}
		else
		{
			nextaddr = ((Block*)nextaddr)->next;
			i++;
		}
	}
	return nextaddr;
}
//POP THE DATA ADDR
void* POP(Blocklist *list, int number)
{	
	////printf("POP LIST[%d] <%p>\n",number,list);	
	Block* targetblock = (Block*)SEARCH(list, number);
	if (number > 0 && number < list->list_size)
	{
		Block* beforeblock = (Block*)((targetblock)->before);
		beforeblock->next = targetblock->next;
		void* nextaddr = list->address;
		////printf("[0]current moving to -> %p\n", nextaddr);
		for (int i = 1; i < list->list_size -1; i++)
		{
			nextaddr = ((Block*)nextaddr)->next;
			////printf("[%d]current moving to -> %p\n", i,nextaddr);
		}
		list->current = nextaddr;
	}
	else if (number == 0 && list->list_size-1 > 0)
	{
		list->address = ((Block*)targetblock)->next;
		void* nextaddr = list->address;
		////printf("[0]current moving to -> %p\n", nextaddr);
		for (int i = 1; i < list->list_size-1; i++)
		{
			nextaddr = ((Block*)nextaddr)->next;
			////printf("[%d]current moving to -> %p\n", i,nextaddr);
		}
		list->current = nextaddr;
	}
	else if (number == 0)
	{
		list->current = nullptr;
	}
	else
	{
		//printf(RED"| BLOCK LIST | %p | [%d] IS WRONG! | LIST SIZE = %d" _RESET "\n", list, number, list->list_size);
		return nullptr;
	}
	list->list_size--;
	void* dataaddr = targetblock->Data_address;
	woolloc_free(targetblock);
	//printf(GREEN"| BLOCK LIST | %p | POP | %p " _RESET "\n", list, dataaddr);
	return dataaddr;
}
//PUSH THE NEW BLOCK TO THE END OF THE LIST BY LINKING METHOD
void PUSH(Blocklist *list, void* value_addr)
{
	////printf("PUSH \n List<%p> size [%d] \n %p  current<%p>\n", list, list->list_size, value_addr,list->current);
	Block* newblock = (Block*)woolloc(sizeof(Block));
	newblock->Data_address = value_addr;
	//////printf("PUSH %p connect with %p\n", newblock, newblock->Data_address);
	if (list->list_size == 0)
	{
		newblock->before = NULL;
		newblock->next = NULL;
		list->address = newblock;
		list->current = newblock;
		list->list_size = 1;
	}
	else
	{
		Block* currentblock = (Block*)list->current;
		currentblock->next = newblock;
		//////printf("[%d]%p ->", list->list_size -1 , currentblock);
		//////printf("[%d]%p connect with %p \n",
		//	list->list_size, (Block*)currentblock->next
		//	, ((Block*)currentblock->next)->Data_address);
		newblock->before = currentblock;
		newblock->next = NULL;
		list->current = currentblock->next;
		list->list_size++;
	}
	//printf(GREEN"| BLOCK LIST | %p | PUSH | %p " _RESET "\n", list, value_addr);
}
//SHOWING THE INFORMATION OF THE BLOCK LIST
void ShowBlocklist(Blocklist *list)
{
	//printf("[BLOCKLIST] <%p> SHOW\n");
	if (list->list_size >0)
	{
		//printf("list length %d\n", list->list_size);

		Block* nextaddr = (Block*)list->address;
		for (int i = 0; i < list->list_size - 1; i++)
		{
			//printf("[%d]%p connect with %p ", i, nextaddr, nextaddr->Data_address);
			//printf("| NEXT-> %p\n", nextaddr->next);
			nextaddr = (Block*)nextaddr->next;
		}
		//printf("[%d]%p connect with", list->list_size - 1, nextaddr);
		//printf(" %p \n", nextaddr->Data_address);
		//printf("CURRENT POINTER %p\n", list->current);
	}
	//printf("\n");
}

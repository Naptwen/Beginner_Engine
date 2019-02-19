#pragma once
#include "Woo_malloc.h"

//
void woolloc_init(void)
{
	//printf("[WOO_MALLOC] \n");
	//printf(" || INIT START\n");
	free_pointer_list_start = (unsigned int)&free_pointer_list[0];
	free_pointer_list_end = (unsigned int)&free_pointer_list[WNUMBER - 1];
	for (int i = 0; i < WNUMBER; i++)
		*(char*)(free_pointer_list_start + i) = NULL;
	//printf(" || WOOLLOC INIT [%d] ---> [%d] BYTE %d \n", free_pointer_list_start, free_pointer_list_end, (free_pointer_list_end - free_pointer_list_start) + 1);
}
//the returened address is the data address
//YOU CAN ATTACHED THE DATA FROM [RETUREND ADDRESS] TO [RETURNED ADDRESS + size - 1]
//BUT MUST REMEMBER DON'T OVER THE SIZE!
//THIS WOOLLOC CAN'T DISTINGUISH HOW MUCH THE OVER SIZE
//YOU CAN FIND THE HEADER BY [RETUREND ADDRESS - sizeof(woo_header)]
//BUT MUST REMEMBER DON"T CHANGE THE HEADER DATA; IT WILL MAKE A LOT OF TROUBLE 
//JUST USING IT FOR CHECKING THE SIZE OR OTHER INFORMATIONS!
void* woolloc(size_t size)
{
	//printf("[WOO_MALLOC]\n");
	//printf(" || woolloc START\n");
	if (free_pointer_list_start == NULL || free_pointer_list_end == NULL)
	{
		//printf(" || <ARAM> WOOLLOC NEED TO BE CREATED\n");
		woolloc_init();
	}

	unsigned int free_pointer = free_pointer_list_start;

	while (free_pointer + size <= free_pointer_list_end)
	{
		woo_header* temp_data_header = (woo_header*)free_pointer;
		if (temp_data_header->used == false)
		{
			if (temp_data_header->size == NULL)
			{
				temp_data_header->used = true; //Is this header use or not;
				temp_data_header->size = size; //Write data_header reviecve datasize =  size;  
				temp_data_header->this_start_pointer = free_pointer + sizeof(woo_header); //Write data_header start point = &free pointer ex)ptr 0x0008
				temp_data_header->this_end_pointer = free_pointer + sizeof(woo_header) + size - 1; //Write data_header end point = &free pointer + size; 
																								   //ex) if data type is char(1byte),give 1byte from 0x0008 to 0x0008 1 byte
																								   //ex) if data type is double(8byte),give 8byte from 0x0008 to 0x000F 8 byte
				//printf(" || |FINED FREE MEMORY|\n");
				//printf(" || [GIVE] HEADER [%d] PTR[%d] -- - untill[%d]\n", (unsigned int)temp_data_header, temp_data_header->this_start_pointer, temp_data_header->this_end_pointer);
				return (void*)temp_data_header->this_start_pointer;
				break;
			}
			else if (temp_data_header->size >= size)
			{
				temp_data_header->used = true;
				//printf(" || |REUSING USED MEMORY| : <THE SIZE IS ENOUGH>\n");
				//printf(" || [GIVE] HEADER [%d] PTR [%d] --- untill [%d]\n", (unsigned int)temp_data_header, temp_data_header->this_start_pointer, temp_data_header->this_end_pointer);
				return (void*)temp_data_header->this_start_pointer;
				break;
			}
			else if (temp_data_header->size < size)
			{
				free_pointer = temp_data_header->this_end_pointer + 1;
				//printf(" || |REUSING USED MEMORY| : <THE SIZE IS INSUFFICIEN>\n");
				//printf(" || |NOT ENGOUHT SIZE MEMORY| [%d]-> [NEXT] MEMORY [%d] --- untill [%d]\n", (unsigned int)temp_data_header, temp_data_header->this_start_pointer, temp_data_header->this_end_pointer);
			}
		}
		else if (temp_data_header->used == true)
		{
			free_pointer = temp_data_header->this_end_pointer + 1;
			//printf(" || |USING MEMORY| [%d]-> [NEXT] MEMORY [%d] --- untill [%d]\n", (unsigned int)temp_data_header, temp_data_header->this_start_pointer, temp_data_header->this_end_pointer);
		}
	}
	//printf(" || <ERROR> Couldn't find any free memory pointer \n");
	return NULL;
}
//FREE AND RE ARRANGE
void woolloc_free(void* addr)
{
	//printf("[WOO_MALLOC]\n");
	//printf(" || FREE START \n");
	unsigned int address = (unsigned int)addr - sizeof(woo_header);
	woo_header* temp_data_header = (woo_header*)address;
	if ((unsigned int)addr>free_pointer_list_end || (unsigned int)addr<free_pointer_list_start)
	{
		//printf(" || [%d] THIS IS NOT INCLUDE IN MY MALLOC\n", address);
	}
	else if (temp_data_header->used == true)
	{
		unsigned int delete_size = temp_data_header->this_end_pointer - temp_data_header->this_start_pointer + 1;
		unsigned int ending_point = temp_data_header->this_end_pointer + 1;
		temp_data_header->used = false;
		for (unsigned int i = (unsigned int)addr; i < ending_point; i++)
		{
			*(char*)(i) = NULL;
		}
		//printf(" || FREE Data[%d](Header[%d])\n", temp_data_header->this_start_pointer, (unsigned int)temp_data_header);
		//woolloc_arange(ending_point, delete_size);
	}
}
//WOLLOC SHOW SHOWS HOW MANY MEMORYS ARE USING NOW AND THE EMPTY UESE MEMORYS
//IT CAN'T SHOW THE DATA'S INFORMATION SUCH AS VALUE, IT JUST SHOW THE MEMORY INFORMATION
void woolloc_show(void)
{
	//printf("\n---------------<SHOW WOOLLOC>\n");
	if (free_pointer_list_start == NULL || free_pointer_list_end == NULL)
	{
		//printf("[WOO_MALLOC] : WOOLLOC IS EMPTY\n");
	}
	unsigned int i = free_pointer_list_start;
	while (i < free_pointer_list_end)
	{
		woo_header* tempheader = (woo_header*)i;
		if (tempheader->used == true)
		{
			cout << "[WOO_MALLOC] + USING DATA" << endl;
			cout << " || H)SHOW HEADER ADDR      : " << (unsigned int)tempheader << endl;
			cout << " || H)SHOW USED             : " << (bool)(tempheader->used) << endl;
			cout << " || H)SHOW SIZE             : " << tempheader->size << endl;
			cout << " || H)SHOW HEADER SIZE      : " << sizeof(woo_header) << endl;
			cout << " || H)SHOW HEADER END ADDR  : " << (unsigned int)(tempheader + sizeof(woo_header) - 1) << endl;
			cout << " || D)SHOW DATA START ADDR  : " << tempheader->this_start_pointer << endl;
			cout << " || D)SHOW DATA END ADDR    : " << tempheader->this_end_pointer << endl;
			i = tempheader->this_end_pointer + 1;
		}
		else if (tempheader->used == false)
		{
			if (tempheader->size != NULL && tempheader->this_end_pointer != NULL)
			{
				cout << "[WOO_MALLOC] - UNUSING DATA" << endl;
				cout << " || H)SHOW HEADER ADDR      : " << (unsigned int)tempheader << endl;
				cout << " || H)SHOW USED             : " << (bool)(tempheader->used) << endl;
				cout << " || H)SHOW SIZE             : " << tempheader->size << endl;
				cout << " || H)SHOW HEADER SIZE      : " << sizeof(woo_header) << endl;
				cout << " || H)SHOW HEADER END ADDR  : " << (unsigned int)(tempheader + sizeof(woo_header) - 1) << endl;
				cout << " || D)SHOW DATA START ADDR  : " << tempheader->this_start_pointer << endl;
				cout << " || D)SHOW DATA END ADDR    : " << tempheader->this_end_pointer << endl;
				i = tempheader->this_end_pointer + 1;
			}
			else
			{
				i++;
			}
		}
	}
	//printf("---------------<SHOW WOOLLOC END>\n");
}
void* woolloc_resize(void* pointer, size_t size)
{
	//printf("[WOO_MALLOC]\n");
	//printf(" || RESIZE START\n");
	if ((unsigned int)pointer <free_pointer_list_start && (unsigned int)pointer >free_pointer_list_end)
	{
		//printf(" || <EEROR>[%d] pointer is not allocated by woolloc\n", (unsigned int)pointer);
		return pointer;
	}
	else
	{
		//printf(" || [%d]resizing. . .\n", (unsigned int)pointer);
		void* temp = woolloc(size);
		woolloc_copy(pointer, temp, size);
		woolloc_free(pointer);
		//printf(" || resizing FINISHED GIVE [%d] \n", (unsigned int)temp);
		return temp;
	}
}
void woolloc_copy(void* sender, void* receiver, size_t size)
{
	//printf("[WOO_MALLOC]\n");
	//printf(" || WOOLOC COPY STARTING\n");
	if ((unsigned int)receiver <free_pointer_list_start && (unsigned int)receiver >free_pointer_list_end)
	{
		//printf(" || <EEROR> receiver is not in the woolloc memory\n");
	}
	else
	{
		//printf(" || COPY from (sender)[%d] to (receiver)[%d] SIZE = %d \n", (unsigned int)sender, (unsigned int)receiver, size);
		woo_header* temp_header = (woo_header*)((unsigned int)receiver - sizeof(woo_header));
		if (temp_header->size < size)
		{
			//printf(" || <EEROR> the size is overflower\n");
		}
		else
		{
			for (unsigned int i = 0; i < size; i++)
			{
				*(char*)((unsigned int)receiver + i) = *(char*)((unsigned int)sender + i);
			}
			//printf(" || COPY FINISH\n");
		}
	}
}

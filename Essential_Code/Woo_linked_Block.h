#pragma once
#include <stdio.h>
#include "Woo_malloc.h"

#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define PUPLE "\x1b[35m"
#define CYAN    "\x1b[36m"
#define _RESET   "\x1b[0m"

struct Block
{
	void* next;
	void* before;
	Block();
	~Block();
	void* Data_address;

};
struct Blocklist
{
	void* address = 0;
	void* current = 0;
	int list_size = 0;
	Blocklist();
	~Blocklist();
};
void* SEARCH(Blocklist *list, int number);
void* POP(Blocklist *list, int number);
void PUSH(Blocklist *list, void* value_addr);
void ShowBlocklist(Blocklist *list);

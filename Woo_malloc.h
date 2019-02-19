#pragma once
#include<stdio.h>
#include<iostream>
#define WNUMBER 1250000 // 125000 BYTE = 1000 KB = 1MB , 10MB
using namespace std;

struct woo_header
{
	// use 1 not 0
	bool used;
	//DATA SIZE
	size_t size;
	//DATA start POINTER
	unsigned int this_start_pointer;
	//DATA END POINTER
	unsigned int this_end_pointer;

};//malloc+ size

static const int header_size = sizeof(woo_header);
static unsigned int free_pointer_list_start; // start of list ptr addr
static unsigned int free_pointer_list_end;   // end of list ptr addr
static char free_pointer_list[WNUMBER]; //  GIVE STATIC MEMORY to handling ,instead of finding heap memory -> it is need direct method of computer memory controlling

										//functioin
void woolloc_show(void);
void woolloc_init(void);
void woolloc_free(void* addr);
void* woolloc(size_t size);
void* woolloc_resize(void* pointer, size_t size);
void woolloc_copy(void* sender, void* receiver, size_t size);
#ifndef _MEM_H
#define _MEM_H
#include <sys.h>

#define MEMORY_POOL_SIZE 0x1600

#define MAX_ALLOCATE_NUM 100

typedef struct mem_unit{
	void *pointer;
	uint32_t start_address;
	uint32_t size;
}mem_unit;

void *mem_alloc(uint32_t size);
void mem_free(void *pointer);
void mem_free_all();

void *memalloc_bitmap(uint32_t size);
void memfree_all();

#endif

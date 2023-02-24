#include "mem.h"
#include "delay.h"
#include "dma.h"
#include "rng.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

mem_unit u[MAX_ALLOCATE_NUM];
uint8_t Memory_Pool[MEMORY_POOL_SIZE];

uint32_t bitmap_a[MEMORY_POOL_SIZE / 32] = {0};
uint32_t bitmap_d[MEMORY_POOL_SIZE / 32] = {0};
uint32_t bitmap_p[MEMORY_POOL_SIZE / 32] = {0};

uint8_t allocate_num = 0;

uint8_t max_try_times = 10;

void *mem_alloc(uint32_t size)
{
	if (allocate_num >= MAX_ALLOCATE_NUM)
		return NULL;
	uint8_t is_success;
	uint16_t s_addr;
	do
	{
		is_success = 1;
		s_addr = RNG_Get_RandomNum() % (MEMORY_POOL_SIZE - size + 1);
		for (uint8_t i = 0; i < allocate_num; i++)
		{
			if (MAX(u[i].start_address, s_addr) < 
				MIN(u[i].start_address + u[i].size - 1, s_addr + size -1))
			{
				is_success = 0;
			}
		}
	}while(!is_success);
	
	allocate_num++;
	u[allocate_num - 1].start_address = s_addr;
	u[allocate_num - 1].size = size;
	u[allocate_num - 1].pointer = Memory_Pool + s_addr;
	return u[allocate_num - 1].pointer;
}

void mem_free(void *pointer)
{
	for (int i = 0; i < allocate_num; i++)
	{
		if (u[i].pointer == pointer)
		{
			memcpy(&u[i], &u[i + 1], sizeof(mem_unit) * (allocate_num - i -1));
			break;
		}
	}
	allocate_num--;
}

void mem_free_all()
{
	memset(u, 0, MAX_ALLOCATE_NUM);
	allocate_num = 0;
}

void *memalloc_bitmap(uint32_t size)
{
	uint8_t is_success;
	uint16_t s_addr;
	do
	{
		is_success = 1;
		s_addr = rand() % (MEMORY_POOL_SIZE - size + 1);
		uint8_t s_num = s_addr / 32;
		uint8_t s_bit = s_addr % 32;
		uint8_t e_num = (s_addr + size - 1) / 32;
		uint8_t e_bit = (s_addr + size - 1) % 32;
		if (s_num == e_num)
		{
			memset(&bitmap_p[s_num], 0, 1);
			memset(&bitmap_d[s_num], 0, 1);
			bitmap_p[s_num] = ~(0xFFFFFFFF << (32 - s_bit)) & (0xFFFFFFFF << (31 - e_bit)); 
			bitmap_d[s_num] = bitmap_a[s_num] | bitmap_p[s_num];
			bitmap_d[s_num] = bitmap_d[s_num] ^ bitmap_a[s_num];
			if (bitmap_d[s_num] != bitmap_p[s_num])
			{
				is_success = 0;
			}
			else
			{
				bitmap_a[s_num] = bitmap_d[s_num];
			}
		}
		else
		{
			memset(&bitmap_p[s_num], 0, e_num - s_num + 1);
			memset(&bitmap_d[s_num], 0, e_num - s_num + 1);
			bitmap_p[s_num] = ~(0xFFFFFFFF << (32 - s_bit));
			bitmap_p[e_num] = 0x80000000 >> e_bit;
			for (int i = s_num + 1; i < e_num; i++)
			{
				bitmap_p[i] = 0xFFFFFFFF;
			}
			for (int i = s_num; i <= e_num; i++)
			{
				bitmap_d[i] = bitmap_a[i] | bitmap_p[i];
				bitmap_d[i] = bitmap_d[i] ^ bitmap_a[i];
				if (bitmap_d[i] != bitmap_p[i])
				{
					//DMA_printf((char *)"%X %X %X,\r\n", bitmap_a[i], bitmap_d[i], bitmap_p[i]);
					is_success = 0;
					break;
				}
			}
			if (is_success)
			{
				memcpy(&bitmap_a[s_num], &bitmap_d[s_num], e_num - s_num + 1);
			}
		}
	}while(!is_success);
	return Memory_Pool + s_addr;
}

void memfree_all()
{
	memset(bitmap_a, 0, MEMORY_POOL_SIZE / 32);
	
	for (int i = 0; i < MEMORY_POOL_SIZE / 32; i++)
	{
		bitmap_a[i] = 0;
		//DMA_printf((char *)"%d:%X,\r\n", i, bitmap_a[i]);
	}
}

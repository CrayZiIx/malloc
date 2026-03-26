#include "malloc.h"
#include <stddef.h>
#include <stdio.h>

void show_alloc_mem(void){
	t_zone	*tmp;
	t_block *block;
	size_t	total;
	int		header_flag = 0;
	void	*start;
	void	*end;

	tmp = g_malloc.zones;
	total = 0;
	while (tmp != NULL){
		block = tmp->blocks;
		if (!header_flag && tmp->type == ZONE_LARGE) {
			printf("LARGE : %p\n", (void *)tmp);
			header_flag = 1;
		} else if (!header_flag && tmp->type == ZONE_TINY) {
			printf("TINY : %p\n", (void *)tmp);
			header_flag = 1;
		} else if (!header_flag && tmp->type == ZONE_SMALL) {
			printf("SMALL : %p\n", (void *)tmp);
			header_flag = 1;
		}
		while (block->next != NULL) {
			if (!block->free) {
				start = (void *)(block + 1);
				end = (void *)((char *)(block + 1) + block->size);
				printf("%p - %p : %zu bytes\n", start, end, block->size);
				total += block->size;
			}
			block = block->next;
		}

		tmp = tmp->next;
	}
	printf("Total : %zu bytes\n", total);
}

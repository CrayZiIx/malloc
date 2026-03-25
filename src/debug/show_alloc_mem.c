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
		if (tmp->type == ZONE_LARGE) {
			block = tmp->blocks;
			if (!header_flag) {
				printf("LARGE : %p\n", (void *)tmp);
				header_flag = 1;
			}
			start = (void *)(block + 1);
			end = (void *)((char *)(block + 1) + block->size);
			printf("%p - %p : %zu bytes\n", start, end, block->size);
			total += block->size;
		}
		tmp = tmp->next;
	}
	printf("Total : %zu bytes\n", total);
}

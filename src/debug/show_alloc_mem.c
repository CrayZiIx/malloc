#include "malloc.h"
#include <stddef.h>
#include <stdio.h>


static size_t print_block(t_block *block);
static size_t show_zone_type(t_zone_type type, const char *label);

void show_alloc_mem(void){
	size_t total;

	total = 0;
	total += show_zone_type(ZONE_TINY, "TINY");
	total += show_zone_type(ZONE_SMALL, "SMALL");
	total += show_zone_type(ZONE_LARGE, "LARGE");
	printf("Total : %zu bytes\n", total);
}

static size_t print_block(t_block *block) {
	void *start;
	void *end;

	if (block->free)
		return (0);
	start = (void *)(block + 1);
	end = (void *)((char * )(block + 1) + block->size);
	printf("%p - %p : %zu bytes\n", start, end, block->size);
	return (block->size);
}

static size_t show_zone_type(t_zone_type type, const char *label) {
	t_zone	*zone;
	t_block	*block;
	size_t	subtotal;
	int		printed_header;

	zone = g_malloc.zones;
	subtotal = 0;
	printed_header = 0;
	while (zone != NULL) {
		if (zone->type == type) {
			if (!printed_header) {
				printf("%s : %p\n", label, (void *)zone);
				printed_header = 1;
			}
			block = zone->blocks;
			while (block != NULL) {
				subtotal += print_block(block);
				block = block->next;
			}
		}
		zone = zone->next;
	}
	return (subtotal);
}

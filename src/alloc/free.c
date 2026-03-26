#include "malloc.h"
#include <sys/mman.h>

void free(void *ptr){
	t_block *block;
	t_zone	*zone;
	if (ptr == NULL)
		return ;
	// recover the block from the user pointer
	block = ((t_block *)ptr) - 1;
	zone = block->zone;

	if (zone->type == ZONE_LARGE) {
		if (zone->prev != NULL)
			zone->prev->next = zone->next;
		else
			g_malloc.zones = zone->next;
		if (zone->next != NULL)
			zone->next->prev = zone->prev;

		munmap(zone, zone->size);
	} else{
		// pooled zones are kept mapped for reuse, even when fully free (may change)
		block->free = 1;
		merge_block(block);
	}
}

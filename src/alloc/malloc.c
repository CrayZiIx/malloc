#include "malloc.h"
#include <sys/mman.h>


void *malloc(size_t size) {
	size_t		aligned_size;
	size_t		mapping_size;
	t_zone_type type;
	t_zone		*zone;
	t_block		*block;

	aligned_size = align_size(size, MALLOC_ALIGNMENT);
	type = get_zone_type(aligned_size);
	if (type == ZONE_LARGE) {
		mapping_size = sizeof(t_zone) + sizeof(t_block) + aligned_size;
		mapping_size = round_up_to_page(mapping_size, get_page_size());

		zone = mmap(NULL, mapping_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (zone == MAP_FAILED)
			return (NULL);

		block = (t_block *)(zone + 1);

		zone->size = mapping_size;
		zone->type = ZONE_LARGE;
		zone->prev = NULL;
		zone->next = g_malloc.zones;
		zone->blocks = block;

		block->size = aligned_size;
		block->free = 0;
		block->zone = zone;
		block->next = NULL;
		block->prev = NULL;

		if (g_malloc.zones != NULL)
			g_malloc.zones->prev = zone;
		g_malloc.zones = zone;

		return ((void *)(block + 1));
	} else {
		block = find_free_block(type, aligned_size);
		if (block == NULL) {
			zone = create_zone(type);
			if (zone == NULL)
				return (NULL);
			block = zone->blocks;
		}
		split_block(block, aligned_size);
		block->free = 0;
		return ((void *)(block + 1));

	}


}

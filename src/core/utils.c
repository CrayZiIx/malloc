#include "malloc.h"
#include <stddef.h>
#include <unistd.h>

size_t get_page_size(void) {
	long	page_size;

	if (g_malloc.page_size != 0)
		return (g_malloc.page_size);
	page_size = sysconf(_SC_PAGESIZE);
	if (page_size <= 0)
		return (0);
	g_malloc.page_size = (size_t)page_size;
	return (g_malloc.page_size);
}


// the following func can overflow with huge size values for now.
// use for payload alignment
size_t	align_size(size_t size, size_t alignment) {
	if (alignment == 0)
		return (size);
	return (((size + alignment - 1) / alignment) * alignment);
}

// use for mmap zone sizing
size_t	round_up_to_page(size_t size, size_t page_size) {
	if (page_size == 0)
		return (size);
	return (((size + page_size - 1) / page_size) * page_size);
}


t_zone_type	get_zone_type(size_t size){
	if (size <= TINY_MAX) {
		return (ZONE_TINY);
	}
	else if (size <= SMALL_MAX) {
		return (ZONE_SMALL);
	}
	return ZONE_LARGE;
}

size_t		get_zone_size(t_zone_type type){
	size_t payload_size;
	size_t zone_size;
	if (type == ZONE_TINY)
		payload_size = align_size(TINY_MAX, MALLOC_ALIGNMENT);
	else if (type == ZONE_SMALL)
		payload_size = align_size(SMALL_MAX, MALLOC_ALIGNMENT);
	else return (0);
	zone_size = sizeof(t_zone);
	zone_size += MIN_ZONE_BLOCKS * (sizeof(t_block) + payload_size);
	return (round_up_to_page(zone_size, get_page_size()));
}

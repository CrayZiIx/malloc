#include "malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>

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
	// return 0 if the addition would overflow. (0 = invalid computed size)
	if (size > SIZE_MAX - (alignment - 1))
		return (0);
	return (((size + alignment - 1) / alignment) * alignment);
}

// use for mmap zone sizing
size_t	round_up_to_page(size_t size, size_t page_size) {
	if (page_size == 0)
		return (size);
	if (size > SIZE_MAX - (page_size - 1))
		return (0);
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
	size_t	block_span;
	size_t	payload_size;
	size_t	zone_size;
	if (type == ZONE_TINY)
		payload_size = align_size(TINY_MAX, MALLOC_ALIGNMENT);
	else if (type == ZONE_SMALL)
		payload_size = align_size(SMALL_MAX, MALLOC_ALIGNMENT);
	else
		return (0);
	if (payload_size == 0)
		return (0);
	if (sizeof(t_block) > SIZE_MAX - payload_size)
		return (0);
	block_span = sizeof(t_block) + payload_size;
	if (MIN_ZONE_BLOCKS > SIZE_MAX / block_span)
		return (0);
	zone_size = MIN_ZONE_BLOCKS * block_span;
	if (sizeof(t_zone) > SIZE_MAX - zone_size)
		return (0);
	zone_size += sizeof(t_zone);
	return (round_up_to_page(zone_size, get_page_size()));
}

t_zone	*create_zone(t_zone_type type) {
	t_zone	*zone;
	t_block	*block;
	size_t	zone_size;

	if (type == ZONE_LARGE)
		return (NULL);
	zone_size = get_zone_size(type);
	if (zone_size == 0)
		return (NULL);
	zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (zone == MAP_FAILED)
		return (NULL);

	block = (t_block *)(zone + 1);

	zone->size = zone_size;
	zone->type = type;
	zone->blocks = block;

	insert_zone(zone);

	block->size = zone_size - sizeof(t_zone) - sizeof(t_block);
	block->free = 1;
	block->zone = zone;
	block->prev = NULL;
	block->next = NULL;

	return zone;
}

t_block	*find_free_block(t_zone_type type, size_t size) {
	t_zone	*zone;
	t_block	*block;

	zone = g_malloc.zones;
	while (zone != NULL) {
		if (zone->type == type){
			block = zone->blocks;
			while (block != NULL) {
				if (block->free == 1 && block->size >= size)
					return (block);
				block = block->next;
			}
		}
		zone = zone->next;
	}
	return (NULL);
}

void split_block(t_block *block, size_t size) {
	t_block *new_block;

	if (block->size <= size + sizeof(t_block))
		return ;
	new_block = (t_block *)((char *)(block + 1) + size);

	new_block->size = block->size - size - sizeof(t_block);
	new_block->free = 1;
	new_block->zone = block->zone;
	new_block->prev = block;
	new_block->next = block->next;

	block->size = size;
	if (block->next != NULL)
		block->next->prev = new_block;
	block->next = new_block;
}

void merge_block(t_block *block) {
	t_block *prev;
	t_block *next;

	if (block == NULL)
		return ;
	if (block->next != NULL && block->next->free) {
		next = block->next;
		block->size += sizeof(t_block) + next->size;
		block->next = next->next;
		if (block->next != NULL)
			block->next->prev = block;
	}
	if (block->prev != NULL && block->prev->free) {
		prev = block->prev;
		prev->size += sizeof(t_block) + block->size;
		prev->next = block->next;
		if (block->next != NULL)
			block->next->prev = prev;
	}
}

void	insert_zone(t_zone *zone) {
	t_zone	*current;

	if (zone == NULL)
		return ;
	if (g_malloc.zones == NULL) {
		zone->prev = NULL;
		zone->next = NULL;
		g_malloc.zones = zone;
		return ;
	}
	if (zone < g_malloc.zones) {
		zone->prev = NULL;
		zone->next = g_malloc.zones;
		g_malloc.zones->prev = zone;
		g_malloc.zones = zone;
		return ;
	}
	current = g_malloc.zones;
	while (current->next != NULL && current->next < zone)
		current = current->next;
	zone->next = current->next;
	zone->prev = current;
	if (current->next != NULL)
		current->next->prev = zone;
	current->next = zone;
}

int zone_is_empty(t_zone *zone) {
	t_block *block;

	if (zone == NULL)
		return (0);

	block = zone->blocks;
	while (block != NULL) {
		if (!block->free)
			return (0);
		block = block->next;
	}
	return (1);
}

void *ft_memcpy(void *dst, const void *src, size_t n) {
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n) {
		d[i] = s[i];
		i++;
	}
	return (dst);
}

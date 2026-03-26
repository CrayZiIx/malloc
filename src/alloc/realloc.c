#include "malloc.h"
#include <string.h>

void *realloc(void *ptr, size_t size){
	t_block *block;
	void 	*new_ptr;
	size_t	aligned_size;
	size_t	copy_size;

	if (ptr == NULL)
			return(malloc(size));
	if (size == 0) {
		free(ptr);
		return (NULL);
	}

	block = ((t_block *)ptr - 1);
	aligned_size = align_size(size, MALLOC_ALIGNMENT);

	// if the existing block is big enough, keep it.
	if (block->size >= aligned_size)
		return (ptr);

	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);
	if (block->size < aligned_size)
		copy_size = block->size;
	else
		copy_size = aligned_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	free(ptr);
	return (new_ptr);
}

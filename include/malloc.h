#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h>
//define
# define MALLOC_ALIGNMENT 16
# define TINY_MAX 64
# define SMALL_MAX 1024
# define MIN_ZONE_BLOCKS 100

// typedef
// zone type.
typedef enum e_zone_type {
	ZONE_TINY,
	ZONE_SMALL,
	ZONE_LARGE
}	t_zone_type;

typedef struct s_block {
	// usable payload size for this block
	size_t			size;
	// whether the block is available
	int				free;
	struct s_zone	*zone;
	struct s_block	*next;
	struct s_block	*prev;
}	t_block;

// equivalent to mmap region. metadata + blocks.
typedef struct s_zone {
	size_t			size;
	t_zone_type		type;
	struct s_zone	*next;
	struct s_zone	*prev;
	t_block			*blocks;
}	t_zone;

// global allocator state.
typedef struct s_malloc {
	t_zone *zones;
	size_t page_size;
}	t_malloc;

// src/alloc/*
void 	*malloc(size_t size);
void 	free(void *ptr);
void 	*realloc(void *ptr, size_t size);
void 	show_alloc_mem(void);
// src/core/utils.c
size_t		get_page_size(void);
size_t		align_size(size_t size, size_t alignment);
size_t		round_up_to_page(size_t size, size_t page_size);
t_zone_type	get_zone_type(size_t size);
size_t		get_zone_size(t_zone_type type);

// g_var
extern t_malloc		g_malloc;

#endif

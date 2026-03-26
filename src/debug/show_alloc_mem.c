#include "malloc.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


static size_t	print_block(t_block *block);
static size_t	show_zone_type(t_zone_type type, const char *label);
static void		putstr(const char *s);
static void		putnbr_size(size_t n);
static void		puthex_ulong(uintptr_t n);
static void		puthex_ptr(void *ptr);

void show_alloc_mem(void){
	size_t total;

	total = 0;
	total += show_zone_type(ZONE_TINY, "TINY");
	total += show_zone_type(ZONE_SMALL, "SMALL");
	total += show_zone_type(ZONE_LARGE, "LARGE");
	putstr("Total : ");
	putnbr_size(total);
	putstr(" bytes\n");
}

static size_t print_block(t_block *block) {
	void *start;
	void *end;

	if (block->free)
		return (0);
	start = (void *)(block + 1);
	end = (void *)((char * )(block + 1) + block->size);
	puthex_ptr(start);
  	putstr(" - ");
  	puthex_ptr(end);
  	putstr(" : ");
  	putnbr_size(block->size);
  	putstr(" bytes\n");
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
			block = zone->blocks;
			while (block != NULL) {
				if (!block->free) {
					if (!printed_header) {
						putstr(label);
						putstr(" : ");
						puthex_ptr((void *)zone);
						putstr("\n");
						printed_header = 1;
					}
					subtotal += print_block(block);
				}
				block = block->next;
			}
		}
		zone = zone->next;
	}
	return (subtotal);
}

static void putstr(const char *s) {
	size_t len;

	len = 0;
	while(s[len] != '\0')
		len++;
	write(1, s, len);
}

static void	putnbr_size(size_t n) {
	char c;

	if (n >= 10)
		putnbr_size(n / 10);
	c = '0' + (n % 10);
	write(1, &c, 1);
}

static void puthex_ulong(uintptr_t n) {
	char *base;
	char c;

	base = "0123456789abcdef";
	if (n >= 16)
		puthex_ulong(n / 16);
	c = base[n % 16];
	write(1, &c, 1);
}

static void puthex_ptr(void *ptr){
	uintptr_t addr;

	addr = (uintptr_t)ptr;
	putstr("0x");
	puthex_ulong(addr);
}

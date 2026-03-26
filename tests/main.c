#include <stdlib.h>
#include <unistd.h>
#include "malloc.h"

static void	putmsg(const char *msg){
	while (*msg)
		write(1, msg++, 1);
}

int main(void){
	// MIN MAX TEST
	// void *tiny_max;
	// void *small_min;
	// void *small_max;
	// void *large_min;

	// tiny_max = malloc(TINY_MAX);
	// small_min = malloc(TINY_MAX + 1);
	// small_max = malloc(SMALL_MAX);
	// large_min = malloc(SMALL_MAX + 1);
	// show_alloc_mem();
	// free(tiny_max);
	// free(small_min);
	// free(small_max);
	// free(large_min);

	void *a;
	void *b;
	void *c;
	void *d;

	a = malloc(128);
	b = malloc(128);
	c = malloc(128);
	show_alloc_mem();

	free(a);
	free(b);
	show_alloc_mem();

	d = malloc(256);
	show_alloc_mem();
	if (d == a)
		putmsg("reuse ok\n");
	else
		putmsg("reuse failed\n");

	free(c);
	free(d);
	show_alloc_mem();

	if (malloc(0) == NULL)
		putmsg("malloc(0) -> NULL)\n");
	else
		putmsg("malloc(0) -> failed policy\n");
	return (0);
}

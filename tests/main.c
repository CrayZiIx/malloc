#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "malloc.h"

int main(void){

	char	*a;
	char	*b;
	char	*c;
	char	*d;
	a = malloc(128);
	b = malloc(128);
	c = malloc(128);
	// printf("after 3 allocation:\n");
	show_alloc_mem();

	free(b);
	// printf("after free(b):\n");
	show_alloc_mem();

	free(a);
	// printf("after free(a):\n");
	show_alloc_mem();

	d = malloc(256);
	// printf("after malloc(256):\n");
	show_alloc_mem();

	free(c);
	free(d);

	if (malloc(0) == NULL)
		write(1, "malloc(0) -> NULL\n", 18);

	// show_alloc_mem();
	return (0);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	// strcpy(ptr, "test malloc");
	// printf("after malloc: %s\n", ptr);
	// ptr = realloc(ptr, 4096);
	// show_alloc_mem();
	// if (ptr == NULL) {
	// 	printf("realloc failed: %s\n", ptr);
	// 	return (1);
	// }
	// printf("after realloc: %s\n", ptr);
	// free(ptr);
	// printf("free done\n");
	// show_alloc_mem();
	return (0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "malloc.h"

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
	printf("a=%p b=%p c=%p\n", a, b, c);
	show_alloc_mem();

	free(a);
	free(b);
	show_alloc_mem();

	d = malloc(256);
	printf("d=%p\n", d);
	show_alloc_mem();

	free(c);
	free(d);
	show_alloc_mem();
	return (0);
}

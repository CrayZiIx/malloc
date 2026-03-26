#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "malloc.h"

int main(void){
	void *tiny_max;
	void *small_min;
	void *small_max;
	void *large_min;

	tiny_max = malloc(TINY_MAX);
	small_min = malloc(TINY_MAX + 1);
	small_max = malloc(SMALL_MAX);
	large_min = malloc(SMALL_MAX + 1);
	show_alloc_mem();
	free(tiny_max);
	free(small_min);
	free(small_max);
	free(large_min);
	return (0);
}

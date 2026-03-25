#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "malloc.h"

int main(void){

	char	*ptr;
	ptr = malloc(128);
	if (ptr == NULL) {
		printf("malloc failed\n");
		return (1);
	}
	show_alloc_mem();
	strcpy(ptr, "test malloc");
	printf("after malloc: %s\n", ptr);
	ptr = realloc(ptr, 4096);
	show_alloc_mem();
	if (ptr == NULL) {
		printf("realloc failed: %s\n", ptr);
		return (1);
	}
	printf("after realloc: %s\n", ptr);
	free(ptr);
	printf("free done\n");
	show_alloc_mem();
	return (0);
}

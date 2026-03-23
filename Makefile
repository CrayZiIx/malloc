ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK := libft_malloc.so

CC := cc
CFLAGS := -Wall -Wextra -Werror -fPIC -Iinclude
LDFLAGS := -shared

SRC := \
		src/alloc/malloc.c \
		src/alloc/free.c \
		src/alloc/realloc.c \
		src/core/global.c \
		src/core/utils.c \
		src/debug/show_alloc_mem.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)
	ln -sf $(NAME) $(LINK)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(LINK)
re: fclean all

.PHONY: all clean fclean re

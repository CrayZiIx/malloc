ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME := libft_malloc_$(HOSTTYPE).so
LINK := libft_malloc.so

CC := cc
CFLAGS := -Wall -Wextra -Werror -fPIC -Iinclude
LDFLAGS := -shared
TEST_CFLAGS := -Wall -Wextra -Werror -Iinclude
TEST_LDFLAGS := -L. -lft_malloc

SRC := \
		src/alloc/malloc.c \
		src/alloc/free.c \
		src/alloc/realloc.c \
		src/core/global.c \
		src/core/utils.c \
		src/debug/show_alloc_mem.c

OBJ := $(SRC:.c=.o)
TEST_SRC := tests/main.c
TEST_BIN := tests/test_malloc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)
	ln -sf $(NAME) $(LINK)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(NAME)
	$(CC) $(TEST_CFLAGS) $(TEST_SRC) $(TEST_LDFLAGS) -o $(TEST_BIN) -g

test_build: $(TEST_BIN)

test: $(TEST_BIN)
	LD_LIBRARY_PATH=. ./$(TEST_BIN)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(LINK) $(TEST_BIN)
re: fclean all

.PHONY: all clean fclean re test test_build

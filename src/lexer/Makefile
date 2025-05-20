CC = cc

CFLAGS = -Wall -Werror -Wextra
CFLAGS += -I./incs -I../libs/libft/incs -I../libs/collector/incs -I.

VPATH = token:token/utils:common

SRCS = lexer.c common.c token_create.c token_str.c token_flags.c

OBJS = $(SRCS:%.c=build/objs/%.o)

NAME = build/libs/lexer.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

build/objs/%.o: %.c
	@mkdir -p build/objs
	@mkdir -p build/libs
	$(CC) $(CFLAGS) -c $< -o  $@

clean:
	@rm -rf build/objs

fclean: clean
	@rm -rf build

re: fclean all

.PHONY: clean

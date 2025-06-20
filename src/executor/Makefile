CC = cc

CFLAGS = -Wall -Werror -Wextra -g
CFLAGS += -I./incs -I../libs/libft/incs -I../libs/collector/incs -I. -I../parser/incs -I..

VPATH = srcs

SRCS = executor.c

OBJS = $(SRCS:%.c=build/objs/%.o)

NAME = build/libs/executor.a

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

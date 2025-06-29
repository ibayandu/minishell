CC = cc

CFLAGS = -Wall -Werror -Wextra -g
CFLAGS += -I.

SRCS = get_next_line.c get_next_line_utils.c

OBJS = $(SRCS:%.c=build/objs/%.o)

NAME = build/libs/get_next_line.a

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

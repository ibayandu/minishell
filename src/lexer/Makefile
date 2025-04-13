CC = cc

CFLAGS = -Wall -Werror -Wextra -Iincludes

VPATH = token:common

SRCS = lexer.c lexer_utils.c token_count.c common.c

OBJS = $(SRCS:%.c=build/%.o)

TARGET = build/bin/lexer.a

$(TARGET): $(OBJS)
	ar rcs $@ $^

build/%.o: %.c
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -c $< -o  $@

clean:
	@rm -rf build

.PHONY: clean
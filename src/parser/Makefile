CC = gcc

CFLAGS = -Wall -Werror -Wextra -Iincludes

VPATH = parser_utils:common

SRCS = parser.c

OBJS = $(SRCS:%.c=build/%.o)

TARGET = build/bin/parser.a

$(TARGET): $(OBJS)
	ar rcs $@ $^

build/%.o: %.c
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build
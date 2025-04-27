CC = cc

CFLAGS = -Wall -Werror -Wextra 
CFLAGS += -I./incs -I..libs/libft/incs

VPATH = token:token/utils:common

SRCS = lexer.c common.c token_create.c token_str.c

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
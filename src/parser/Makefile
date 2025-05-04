CC = cc

CFLAGS = -Wall -Werror -Wextra 
CFLAGS += -Iincs -I../lexer -I../lexer/incs -I../libs/libft/incs

VPATH = parser_utils:parser_utils/parser_funcs:common

SRCS = parser.c helper.c parse_sequence.c parse_redirs.c parse_command.c parse_pipe.c

OBJS = $(SRCS:%.c=build/%.o)

TARGET = build/bin/parser.a

$(TARGET): $(OBJS)
	ar rcs $@ $^

build/%.o: %.c
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build
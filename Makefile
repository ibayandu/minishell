MAKEFLAGS+=--no-print-directory

CC = gcc

CFLAGS = -Wall -Werror -Wextra

VPATH = src

SRCS = main.c

LEXER = lexer/build/bin/lexer.a
PARSER = parser/build/bin/parser.a
LIBFT = lib/libft/libft.a

OBJS = $(SRCS:%.c=src/build/%.o)

NAME = minishell

src/build/%.o: %.c
	@mkdir -p src/build
	$(CC) $(CFLAGS) -c $< -o $@

all:
	@$(MAKE) -C src/lib/libft
	@$(MAKE) -C src/parser
	@$(MAKE) -C src/lexer
	@$(MAKE) program

program: ${NAME}

$(NAME): $(OBJS) $(LEXER) $(PARSER) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf src/build
	@$(MAKE) clean -C src/lexer
	@$(MAKE) clean -C src/parser
	@$(MAKE) clean -C src/lib/libft

fclean: clean
	@rm -rf $(NAME)

re: fclean all

norm:
	norminette

run:
	./$(NAME)

.PHONY:	program all clean fclean re run

MAKEFLAGS+=--no-print-directory

CC = cc

CFLAGS = -Wall -Werror -Wextra -g
CFLAGS += -Isrc/libs/libft/incs -Isrc/lexer -Isrc/lexer/incs -Isrc/parser -Isrc/parser/incs -Isrc/libs/collector/incs

VPATH = src

SRCS = main.c

LEXER = lexer/build/libs/lexer.a
PARSER = parser/build/libs/parser.a
LIBFT = libs/libft/build/libs/libft.a
COLLECTOR = libs/collector/build/libs/collector.a

OBJS = $(SRCS:%.c=src/build/%.o)

NAME = minishell

src/build/%.o: %.c
	@mkdir -p src/build
	$(CC) $(CFLAGS) -c $< -o $@

all:
	@$(MAKE) -C src/libs/collector
	@$(MAKE) -C src/libs/libft
#	@$(MAKE) -C src/parser
	@$(MAKE) -C src/lexer
	@$(MAKE) program

program: ${NAME}

$(NAME): $(OBJS) $(LEXER) $(LIBFT) $(COLLECTOR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm -rf src/build
	@$(MAKE) clean -C src/lexer
#	@$(MAKE) clean -C src/parser
	@$(MAKE) clean -C src/libs/libft
	@$(MAKE) clean -C src/libs/collector

fclean: clean
	@rm -rf $(NAME)

re: fclean all

norm:
	norminette

run:
	./$(NAME)

valgrind:
	valgrind --leak-check=full ./$(NAME)

.PHONY:	program all clean fclean re run

MAKEFLAGS += --no-print-directory

CC = cc

CFLAGS = -Wall -Werror -Wextra -g
IFLAGS = -Isrc -Isrc/libs/libft/incs -Isrc/lexer -Isrc/lexer/incs -Isrc/parser/incs -Isrc/libs/collector/incs -Isrc/executor/incs -Isrc/libs/get_next_line
LFLAGS = -lreadline

VPATH = src

SRCS = main.c decode_prompt.c

LIBFT = libs/libft/build/libs/libft.a
COLLECTOR = libs/collector/build/libs/collector.a
GET_NEXT_LINE = libs/get_next_line/build/libs/get_next_line.a
LEXER = lexer/build/libs/lexer.a
PARSER = parser/build/libs/parser.a
EXECUTOR = executor/build/libs/executor.a


OBJS = $(SRCS:%.c=src/build/%.o)

NAME = minishell

src/build/%.o: %.c
	@mkdir -p src/build
	$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

all:
	@$(MAKE) -C src/libs/libft
	@$(MAKE) -C src/libs/collector
	@$(MAKE) -C src/libs/get_next_line
	@$(MAKE) -C src/lexer
	@$(MAKE) -C src/parser
	@$(MAKE) -C src/executor
	@$(MAKE) program

program: ${NAME}

$(NAME):  $(OBJS) $(EXECUTOR) $(PARSER) $(LEXER) $(LIBFT) $(COLLECTOR) $(GET_NEXT_LINE)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)
clean:
	@rm -rf src/build
	@$(MAKE) clean -C src/libs/libft
	@$(MAKE) clean -C src/libs/collector
	@$(MAKE) clean -C src/libs/get_next_line
	@$(MAKE) clean -C src/lexer
	@$(MAKE) clean -C src/parser
	@$(MAKE) clean -C src/executor

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C src/libs/libft
	@$(MAKE) fclean -C src/libs/collector
	@$(MAKE) fclean -C src/libs/get_next_line
	@$(MAKE) fclean -C src/lexer
	@$(MAKE) fclean -C src/parser
	@$(MAKE) fclean -C src/executor

re: fclean all

norm:
	norminette

run:
	./$(NAME)

leak:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)

.PHONY:	program all clean fclean re run leak

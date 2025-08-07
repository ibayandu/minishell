MAKEFLAGS += --no-print-directory

CC = cc

CFLAGS = -Wall -Werror -Wextra -g
IFLAGS = -I ./ -I ./incs -I ./srcs/executor -I ./srcs/executor/incs -I ./srcs/parser -I ./srcs/parser/incs -I ./srcs/lexer -I ./srcs/lexer/incs -I ./srcs/libs/libft -I ./srcs/libs/libmem -I ./srcs/libs/libgnl -I ./srcs/libs/libhash
LFLAGS = -lreadline

VPATH = srcs

SRCS = minishell.c init.c prompt.c

LIBFT = libs/libft/build/libs/libft.a
LIBMEM = libs/libmem/build/libs/libmem.a
LIBGNL = libs/libgnl/build/libs/libgnl.a
LIBHASH = libs/libhash/build/libs/libhash.a
LEXER = lexer/build/libs/lexer.a
PARSER = parser/build/libs/parser.a
EXECUTOR = executor/build/libs/executor.a

OBJS = $(SRCS:%.c=srcs/build/%.o)

NAME = minishell

srcs/build/%.o: %.c
	@mkdir -p srcs/build
	$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

all:
	@$(MAKE) -C srcs/libs/libft
	@$(MAKE) -C srcs/libs/libmem
	@$(MAKE) -C srcs/libs/libgnl
	@$(MAKE) -C srcs/libs/libhash
	@$(MAKE) -C srcs/lexer
	@$(MAKE) -C srcs/parser
	@$(MAKE) -C srcs/executor
	@$(MAKE) program

program: ${NAME}

$(NAME):  $(OBJS) $(EXECUTOR) $(PARSER) $(LEXER) $(LIBHASH) $(LIBFT) $(LIBGNL) $(LIBMEM)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	@rm -rf srcs/build
	@$(MAKE) clean -C srcs/libs/libft
	@$(MAKE) clean -C srcs/libs/libmem
	@$(MAKE) clean -C srcs/libs/libgnl
	@$(MAKE) clean -C srcs/libs/libhash
	@$(MAKE) clean -C srcs/lexer
	@$(MAKE) clean -C srcs/parser
	@$(MAKE) clean -C srcs/executor

fclean: clean
	@rm -rf $(NAME)
	@$(MAKE) fclean -C srcs/libs/libft
	@$(MAKE) fclean -C srcs/libs/libmem
	@$(MAKE) fclean -C srcs/libs/libgnl
	@$(MAKE) fclean -C srcs/libs/libhash
	@$(MAKE) fclean -C srcs/lexer
	@$(MAKE) fclean -C srcs/parser
	@$(MAKE) fclean -C srcs/executor

re: fclean all

norm:
	norminette

run:
	./$(NAME)

leak:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)

.PHONY:	program all clean fclean re run leak norm

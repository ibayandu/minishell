MAKEFLAGS += --no-print-directory

CC = cc
RM = rm -rf
MKDIR = mkdir -p
MAKE = make -C

IFLAGS = -I ./ -I ./incs -I ./srcs/executor -I ./srcs/executor/incs -I ./srcs/parser -I ./srcs/parser/incs -I ./srcs/lexer -I ./srcs/lexer/incs -I ./srcs/libs/libft -I ./srcs/libs/libmem -I ./srcs/libs/libgnl -I ./srcs/libs/libhash
CFLAGS = -Wall -Werror -Wextra
LFLAGS = -lreadline

VPATH = srcs
SRCS = minishell.c init.c prompt.c escape.c getters.c

LIBFT_DIR = srcs/libs/libft
LIBFT = $(LIBFT_DIR)/build/libs/libft.a

LIBMEM_DIR = srcs/libs/libmem
LIBMEM = $(LIBMEM_DIR)/build/libs/libmem.a

LIBGNL_DIR = srcs/libs/libgnl
LIBGNL = $(LIBGNL_DIR)/build/libs/libgnl.a

LIBHASH_DIR = srcs/libs/libhash
LIBHASH = $(LIBHASH_DIR)/build/libs/libhash.a

LEXER_DIR = srcs/lexer
LEXER = $(LEXER_DIR)/build/libs/lexer.a

PARSER_DIR = srcs/parser
PARSER = $(PARSER_DIR)/build/libs/parser.a

EXECUTOR_DIR = srcs/executor
EXECUTOR = $(EXECUTOR_DIR)/build/libs/executor.a

OUT_DIR = build

OBJS_DIR = $(OUT_DIR)/objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME = minishell

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o  $@ $(IFLAGS)

all: dir libraries $(NAME)

dir:
	@if [ ! -d $(OBJS_DIR) ]; then \
		$(MKDIR) $(OBJS_DIR); \
	fi

libraries:
	@$(MAKE) $(LIBMEM_DIR)
	@$(MAKE) $(LIBGNL_DIR)
	@$(MAKE) $(LIBFT_DIR)
	@$(MAKE) $(LIBHASH_DIR)
	@$(MAKE) $(LEXER_DIR)
	@$(MAKE) $(PARSER_DIR)
	@$(MAKE) $(EXECUTOR_DIR)

$(NAME): $(OBJS) $(EXECUTOR) $(PARSER) $(LEXER) $(LIBHASH) $(LIBFT) $(LIBGNL) $(LIBMEM)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

clean:
	@$(MAKE) $(LIBMEM_DIR) clean
	@$(MAKE) $(LIBGNL_DIR) clean
	@$(MAKE) $(LIBFT_DIR) clean
	@$(MAKE) $(LIBHASH_DIR) clean
	@$(MAKE) $(LEXER_DIR) clean
	@$(MAKE) $(PARSER_DIR) clean
	@$(MAKE) $(EXECUTOR_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	@$(MAKE) $(LIBMEM_DIR) fclean
	@$(MAKE) $(LIBGNL_DIR) fclean
	@$(MAKE) $(LIBFT_DIR) fclean
	@$(MAKE) $(LIBHASH_DIR) fclean
	@$(MAKE) $(LEXER_DIR) fclean
	@$(MAKE) $(PARSER_DIR) fclean
	@$(MAKE) $(EXECUTOR_DIR) fclean
	$(RM) $(NAME)
	$(RM) $(OUT_DIR)

re: fclean all

norm:
	norminette

leak:
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./$(NAME)

.PHONY: all clean fclean re dir leak norm libraries

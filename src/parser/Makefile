CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I ./ -I ./incs -I ../lexer -I ../lexer/incs -I../libs/libmem -I../libs/libft -I../libs/libgnl
CFLAGS = -Wall -Werror -Wextra -g

VPATH = srcs/parser_makers:srcs/parser_utils:srcs/parser_funcs
SRCS = parser.c parse_cmd.c parse_list.c parse_redir.c make_cmd.c make_redir.c \
		make_simple.c make_word.c ft_get_cnt.c ft_get_redir.c ft_heredoc.c \
		ft_panic.c ft_readline.c ft_remquot.c ft_listrev.c

OBJS_DIR = build/objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME_DIR = build/libs
NAME = $(NAME_DIR)/parser.a

$(OBJS_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o  $@ $(IFLAGS)

all: dir $(NAME)

dir:
	@if [ ! -d $(OBJS_DIR) ]; then \
		$(MKDIR) $(OBJS_DIR); \
	fi
	@if [ ! -d $(NAME_DIR) ]; then \
		$(MKDIR) $(NAME_DIR); \
	fi

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) build/objs

fclean:
	$(RM) build

re: fclean all

.PHONY: all clean fclean re dir

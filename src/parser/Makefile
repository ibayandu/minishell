CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I./incs -I../lexer -I../libs/collector/incs -I../libs/libft/incs -I../lexer/incs
CFLAGS = -Wall -Werror -Wextra -g

VPATH = srcs:srcs/makers:srcs/utils:srcs/parsers
SRCS = cmd_parser.c list_parser.c redir_parser.c cmd_maker.c redir_maker.c simple_maker.c \
		word_maker.c ft_listrev.c ft_remquot.c ft_mbrlen.c ft_panic.c ft_get_redir.c \
		ft_get_cnt.c ft_readline.c

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

CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I ./ -I ./incs -I ../libs/libft -I ../libs/libmem
CFLAGS = -Wall -Werror -Wextra -g

VPATH = srcs/common:srcs/token
SRCS = lexer.c token_str.c token_flags.c token_create.c common.c

OBJS_DIR = build/objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME_DIR = build/libs
NAME = $(NAME_DIR)/lexer.a

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
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re dir

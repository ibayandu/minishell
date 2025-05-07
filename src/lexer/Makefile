CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I./incs -I../libs/libft/incs
CFLAGS = -Wall -Werror -Wextra

VPATH = token:token/utils:common
SRCS = lexer.c common.c token_create.c token_str.c

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


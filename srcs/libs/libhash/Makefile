CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I ./ -I ./incs -I ../libft -I ../libmem
CFLAGS = -Wall -Werror -Wextra -g

VPATH = srcs
SRCS = libhash.c hash_utils.c

OBJS_DIR = build/objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME_DIR = build/libs
NAME = $(NAME_DIR)/libhash.a

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

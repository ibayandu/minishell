CC = cc
RM = rm -rf
AR = ar rcs
MKDIR = mkdir -p

IFLAGS = -I ./ -I ./incs -I ../parser -I ../parser/incs -I ../lexer -I ../lexer/incs -I ../libs/libmem -I ../libs/libft -I ../libs/libhash
CFLAGS = -Wall -Werror -Wextra -g

VPATH = srcs/exec_builtin:srcs/exec_funcs:srcs/exec_utils:srcs/expander/expand_funcs:srcs/expander/expand_utils:srcs/expander/expand_funcs/expand_glob
SRCS = executor.c alias.c builtin.c cd.c echo.c env.c exit.c export.c pwd.c unalias.c unset.c execute_builtin.c \
		execute_connect.c execute_simple.c execute_subshell.c apply_redirections.c build_argv.c ft_execvp.c \
		glob_dirs.c glob_list.c glob_filename.c glob_vector.c expand_list.c expand_param.c expand_redir.c \
		expand_word.c dir.c list.c sorter.c split.c string.c variable.c vlist.c glob.c strvec.c pipe_utils.c

OBJS_DIR = build/objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

NAME_DIR = build/libs
NAME = $(NAME_DIR)/executor.a

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

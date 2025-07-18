CC = cc

CFLAGS = -Wall -Werror -Wextra -g
CFLAGS += -I./incs -I../libs/libft/incs -I../libs/collector/incs -I. -I../parser/incs -I.. -I../lexer/incs

VPATH = srcs:srcs/expander:srcs/expander/expands:srcs/expander/helpers:srcs/execute:srcs/utils:srcs/builtin:srcs/builtin/alias:srcs/builtin/export
VPATH += :srcs/execute/execute_simple

SRCS = executor.c ft_execvp.c execute_connect.c execute_simple.c execute_simple_helper.c execute_subshell.c apply_redirections.c build_argv.c ft_execvp.c \
		builtin.c cd.c echo.c env.c exit.c export.c export_helper.c pwd.c unset.c expand_word.c expand_list.c split.c string.c expand_param.c \
		list.c list2.c expand_glob.c dir.c expand_redir.c alias.c alias_helper.c unalias.c

OBJS = $(SRCS:%.c=build/objs/%.o)

NAME = build/libs/executor.a

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

build/objs/%.o: %.c
	@mkdir -p build/objs
	@mkdir -p build/libs
	$(CC) $(CFLAGS) -c $< -o  $@

clean:
	@rm -rf build/objs

fclean: clean
	@rm -rf build

re: fclean all

.PHONY: clean

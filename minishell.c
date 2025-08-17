/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 11:28:26 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "libgnl.h"
#include "libft.h"
#include "libmem.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "minishell.h"
#include "init.h"
#include "prompt.h"

static int	*set_code(int code)
{
	static int	exit_code = 0;

	if (code >= 0)
		exit_code = code;
	return (&exit_code);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	set_code(130);
}

char	*ft_repl(void)
{
	char	*line;

	line = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	if (isatty(STDIN_FILENO))
		line = mem_absorb(readline(get_prompt()));
	else
		line = gnl_one(STDIN_FILENO);
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		mem_free();
		exit(0);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	if (*line)
		add_history(line);
	return (line);
}

int	main(void)
{
	t_command	*cmd;
	char		*cmdline;
	extern char	**environ;

	initialize_shell_variables(environ, set_code(-1));
	cmdline = ft_repl();
	while (cmdline)
	{
		if (init_lexer(ft_strjoin(cmdline, "\n")))
		{
			cmd = parse_inputunit(set_code(-1));
			if (!ft_heredoc(set_code(-1)) && cmd)
				set_code(execute_command(cmd, set_code(-1)));
		}
		cmdline = ft_repl();
	}
	rl_clear_history();
	mem_free();
	return (*set_code(-1));
}

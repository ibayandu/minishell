/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 14:36:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "get_next_line.h"
#include "executor.h"
#include "lexer.h"
#include "parsers.h"
#include "init.c"

void	handle_sigint(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		ft_putchar_fd('\n', STDOUT_FILENO);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

char	*ft_repl(t_minishell *minishell)
{
	char		*line;
	char		*ps1;
	t_variable	*v;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	ps1 = PS1;
	line = NULL;
	v = find_variable("PS1", minishell);
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
	if (isatty(fileno(stdin)))
		line = ft_absorb(readline(ps1));
	// else
	// 	line = ft_absorb(get_next_line(fileno(stdin)));
	if (!line)
	{
		write(STDERR_FILENO, "exit\n", 5);
		ft_free();
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
	t_minishell	*const	minishell = &(t_minishell){0};
	t_command			*cmd;
	char				*cmdline;
	extern char			**environ;

	initialize_shell_variables(environ, minishell);
	cmdline = ft_repl(minishell);
	while (cmdline)
	{
		if (!init_lexer(ft_strjoin(cmdline, "\n")))
			return (1);
		cmd = parse_inputunit(minishell);
		if (minishell->need_here_doc)
			gather_here_documents(minishell);
		if (cmd)
			minishell->last_command_exit_value = execute_command(cmd, minishell);
		cmdline = ft_repl(minishell);
	}
	rl_clear_history();
	ft_free();
	return (minishell->last_command_exit_value);
}




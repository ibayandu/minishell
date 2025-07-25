/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/15 17:48:04 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"
#include "init.c"
#include "lexer.h"
#include "parsers.h"
#include "utils.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

char	*ft_repl(t_minishell *minishell)
{
	char	*line;

	line = NULL;
	setup_signals_main();
	if (isatty(STDIN_FILENO))
		line = ft_absorb(readline(get_prompt(minishell)));
	else
		line = ft_absorb(get_next_line(STDIN_FILENO));
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		ft_free();
		exit(0);
	}
	discard_signals();
	if (*line)
		add_history(line);
	return (line);
}

int	main(void)
{
	t_command	*cmd;
	char		*cmdline;
	extern char	**environ;

	t_minishell *const minishell = &(t_minishell){0};
	initialize_shell_variables(environ, minishell);
	cmdline = ft_repl(minishell);
	while (cmdline)
	{
		if (init_lexer(ft_strjoin(cmdline, "\n")))
		{
			minishell->need_here_doc = 0;
			cmd = parse_inputunit(minishell);
			if (!ft_heredoc(minishell) && cmd)
				minishell->last_command_exit_value = execute_command(cmd,
						minishell);
		}
		if (minishell->last_command_exit_value == 130)
			printf("\n");
		cmdline = ft_repl(minishell);
	}
	rl_clear_history();
	ft_free();
	return (minishell->last_command_exit_value);
}

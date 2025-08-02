/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 22:20:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>
#include "libgnl.h"
#include "libft.h"
#include "libmem.h"
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "minishell.h"
#include "init.c"

char	*ft_repl()
{
	char	*line;

	line = NULL;
	setup_signals_main();
	if (isatty(STDIN_FILENO))
		line = mem_absorb(readline(get_prompt()));
	else
		line = gnl_all(STDIN_FILENO);
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
	int			exit_code;

	initialize_shell_variables(environ, &exit_code);
	cmdline = ft_repl();
	while (cmdline)
	{
		if (init_lexer(ft_strjoin(cmdline, "\n")))
		{
			cmd = parse_inputunit(&exit_code);
			if (!ft_heredoc(&exit_code) && cmd)
				exit_code = execute_command(cmd);
		}
		if (exit_code == 130)
			printf("\n");
		cmdline = ft_repl();
	}
	rl_clear_history();
	mem_free();
	return (exit_code);
}

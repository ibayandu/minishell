/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 07:25:40 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "lexer.h"
#include "parsers.h"
#include "printer.c"
#include "executor.h"
#include "init.c"

char	*ft_repl(void)
{
	char	*line;
	char	*ps1;
	// char	*decoded;

	ps1 = getenv("PS1");
	if (!ps1)
		ps1 = "minishell> ";
	else
	{
		ps1 = ft_strtrim(ps1, "\"'");
		ps1 = decode_prompt(ps1);
		// ps1 = decoded;
	}
	line = ft_absorb(readline(ps1));
	if (line == NULL)
	{
		write(STDERR_FILENO, "exit\n", 5);
		ft_free();
		exit(0);
	}
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
	cmdline = ft_repl();
	printf("%s\n", find_variable_internal("PWD", minishell)->value);
	while (cmdline)
	{
		if (!init_lexer(ft_strjoin(cmdline, "\n")))
			return (1);
		cmd = parse_inputunit(minishell);
		if (minishell->need_here_doc)
			gather_here_documents(minishell);
		if (cmd)
		{
			execute_command(cmd);
			// print_command(cmd, 0, 1);
		}
		cmdline = ft_repl();
	}
	ft_free();
	return (0);
}




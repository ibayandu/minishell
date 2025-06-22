/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 23:51:12 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "executor.h"
#include "lexer.h"
#include "parsers.h"
#include "printer.c"
#include "init.c"

char	*ft_repl(t_minishell *minishell)
{
	char		*line;
	char		*ps1;
	t_variable	*v;

	ps1 = PS1;
	v = find_variable_internal("PS1", minishell);
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
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
	cmdline = ft_repl(minishell);
	while (cmdline)
	{
		if (!init_lexer(ft_strjoin(cmdline, "\n")))
			return (1);
		cmd = parse_inputunit(minishell);
		if (minishell->need_here_doc)
			gather_here_documents(minishell);
		if (cmd)
		{
			execute_command(cmd, minishell);
			// print_command(cmd, 0, 1);
		}
		cmdline = ft_repl(minishell);
	}
	ft_free();
	return (0);
}




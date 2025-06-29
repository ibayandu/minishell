/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 01:19:15 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "get_next_line.h"
#include "executor.h"
#include "lexer.h"
#include "parsers.h"
//#include "printer.c"
#include "init.c"

char	*ft_repl(t_minishell *minishell)
{
	char		*line;
	char		*ps1;
	t_variable	*v;

	ps1 = PS1;
	line = NULL;
	v = find_variable("PS1", minishell);
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
	if (isatty(fileno(stdin)))
		line = ft_absorb(readline(ps1));
	else
		line = ft_absorb(get_next_line(fileno(stdin)));
	if (line == NULL)
	{
		//write(STDERR_FILENO, "exit\n", 5);
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
			minishell->last_command_exit_value = execute_command(cmd, minishell); // print_command(cmd, 0, 1);
		cmdline = ft_repl(minishell);
	}
	ft_free();
	return (0);
}




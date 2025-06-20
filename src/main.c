/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/20 05:03:58 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include "lexer.h"
#include "parsers.h"
#include "printer.c"
#include "executor.h"

char	*ft_repl()
{
	char	*line;

	line = ft_absorb(readline(PS1));
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

	cmdline = ft_repl();
	while (cmdline)
	{
		if (!init_lexer(ft_strjoin(cmdline, "\n")))
			return (1);
		cmd = parse_inputunit(minishell);
		if (minishell->need_here_doc)
			gather_here_documents(minishell);
		if (cmd)
			execute_command(cmd);
		cmdline = ft_repl();
	}
	ft_free();
	return (0);
}



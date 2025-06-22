/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:31 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 07:41:02 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"
#include "execute.h"
#include <errno.h>

static void	child_process(t_simple_cmd *cmd, t_redirect *redirects, char **argv, t_minishell *minishell)
{
	if (redirects)
		apply_redirections(redirects, minishell);
	if (cmd->redirects)
		apply_redirections(cmd->redirects, minishell);
	ft_execvp(argv[0], argv);
	ft_putendl_fd(ft_strjoin(argv[0], ": command not found"), 2);
	exit(127);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (1);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, t_minishell *minishell)
{
	char	**argv;
	pid_t	pid;

	cmd->words = expand_word_list(cmd->words, 0, minishell);
	argv = build_argv(cmd->words);
	if (!argv || !argv[0])
		return (1);
	if (is_builtin(cmd->words->word->word))
	{
		run_builtin(cmd);
		return (0);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		child_process(cmd, redirects, argv, minishell);
		_exit(127);
	}
	else
		return (wait_for_child(pid));
}

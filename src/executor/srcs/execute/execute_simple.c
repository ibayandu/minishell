/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:31 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 13:19:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"
#include "execute.h"
#include <errno.h>

static void	child_process(t_simple_cmd *cmd, t_redirect *redirects, char **argv, t_minishell *minishell)
{
	if (redirects)
		if (apply_redirections(redirects, minishell))
		{
			ft_free();
			exit(1);
		}
	if (cmd->redirects)
		if (apply_redirections(cmd->redirects, minishell))
		{
			ft_free();
			exit(1);
		}
	ft_execvp(argv[0], argv, minishell);
	ft_putendl_fd(ft_strjoin(argv[0], ": command not found"), 2);
	ft_free();
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

int	store_fds(int fds[3])
{
	fds[0] = dup(STDIN_FILENO);
	if (fds[0] < 0)
		return (1);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[1] < 0)
		return (1);
	fds[2] = dup(STDERR_FILENO);
	if (fds[2] < 0)
		return (1);
	return (0);
}

int	restore_fds(int fds[3])
{
	if (dup2(fds[0], STDIN_FILENO) < 0)
		return (1);
	if (close(fds[0]) < 0)
		return (1);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return (1);
	if (close(fds[1]) < 0)
		return (1);
	if (dup2(fds[2], STDERR_FILENO) < 0)
		return (1);
	if (close(fds[2]) < 0)
		return (1);
	return (0);
}

int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, t_minishell *minishell)
{
	int		fds[3];
	int		ret;
	char	**argv;
	pid_t	pid;

	cmd->words = expand_word_list(cmd->words, minishell);
	argv = build_argv(cmd->words);
	if (!argv || !argv[0])
		return (1);
	if (is_builtin(cmd->words->word->word))
	{
		if (store_fds(fds))
			return (1);
		if (apply_redirections(cmd->redirects,minishell))
			return (1);
		ret = run_builtin(cmd, minishell);
		if (restore_fds(fds))
			return (1);
		return (ret);
	}
	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
		child_process(cmd, redirects, argv, minishell);
	else
		return (wait_for_child(pid));
	return (127);
}

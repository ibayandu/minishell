/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:31 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:38:32 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_simple.h"

static int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
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

static int	execute_builtin_if_possible(t_simple_cmd *cmd,
		t_minishell *minishell, int fds[3])
{
	if (!cmd->words || !cmd->words->word || !cmd->words->word->word)
		return (-1);
	if (!is_builtin(cmd->words->word->word))
		return (-1);
	if (store_fds(fds))
		return (1);
	if (apply_redirections(cmd->redirects, minishell))
	{
		restore_fds(fds);
		return (1);
	}
	if (restore_fds(fds))
		return (1);
	return (run_builtin(cmd, minishell));
}

int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects,
		t_minishell *minishell)
{
	int		fds[3];
	int		ret;
	pid_t	pid;

	cmd->words = expand_word_list(cmd->words, minishell);
	ret = execute_builtin_if_possible(cmd, minishell, fds);
	if (ret != -1)
		return (ret);
	discard_signals();
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		child_process(cmd, redirects, minishell);
	return (wait_for_child(pid));
}

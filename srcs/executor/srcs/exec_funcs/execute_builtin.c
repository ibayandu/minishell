/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 20:23:42 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/16 23:54:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "libmem.h"
#include "libft.h"
#include "exec_utils.h"
#include "exec_builtin.h"
#include "exec_funcs.h"

static int	store_fds(int fds_store[3])
{
	fds_store[0] = dup(STDIN_FILENO);
	if (fds_store[0] < 0)
		return (1);
	fds_store[1] = dup(STDOUT_FILENO);
	if (fds_store[1] < 0)
		return (1);
	fds_store[2] = dup(STDERR_FILENO);
	if (fds_store[2] < 0)
		return (1);
	return (0);
}

static int	restore_fds(int fds_store[3])
{
	if (dup2(fds_store[0], STDIN_FILENO) < 0)
		return (1);
	if (close(fds_store[0]) < 0)
		return (1);
	if (dup2(fds_store[1], STDOUT_FILENO) < 0)
		return (1);
	if (close(fds_store[1]) < 0)
		return (1);
	if (dup2(fds_store[2], STDERR_FILENO) < 0)
		return (1);
	if (close(fds_store[2]) < 0)
		return (1);
	return (0);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	mem_free();
	exit(130);
}

static int	child_builtin(t_simple_cmd *cmd, t_redirect *redirects,
	int *exit_code)
{
	pid_t	pid;
	int		ret;

	pid = fork();
	if (!pid)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, handle_sigint);
		if (apply_redirections(redirects, exit_code))
		{
			mem_free();
			exit(1);
		}
		if (apply_redirections(cmd->redirects, exit_code))
		{
			mem_free();
			exit(1);
		}
		ret = run_builtin(cmd);
		mem_free();
		exit (ret);
	}
	return (pid);
}

int	execute_builtin(t_simple_cmd *cmd, t_redirect *redirects, int *exit_code)
{
	int		ret;
	int		fds_store[3];

	ret = 0;
	if (!cmd->words || !cmd->words->word || !cmd->words->word->word)
		return (-1);
	if (!is_builtin(cmd->words->word->word))
		return (-1);
	if (redirects && redirects->flags)
		return (child_builtin(cmd, redirects, exit_code));
	if (store_fds(fds_store))
		return (1);
	if (apply_redirections(redirects, exit_code))
		return (restore_fds(fds_store), 1);
	if (apply_redirections(cmd->redirects, exit_code))
		return (restore_fds(fds_store), 1);
	ret = run_builtin(cmd);
	if (restore_fds(fds_store))
		return (1);
	return (ret);
}

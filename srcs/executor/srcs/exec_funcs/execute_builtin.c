/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 20:23:42 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/07 03:26:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "exec_utils.h"
#include "exec_builtin.h"
#include "exec_funcs.h"

static int	store_fds(int fds[3])
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

static int	restore_fds(int fds[3])
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

static int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	return (!ft_strncmp(name, "cd", 3) || !ft_strncmp(name, "export", 7)
		|| !ft_strncmp(name, "unset", 6) || !ft_strncmp(name, "exit", 5)
		|| !ft_strncmp(name, "echo", 5) || !ft_strncmp(name, "env", 4)
		|| !ft_strncmp(name, "pwd", 4) || !ft_strncmp(name, "alias", 6)
		|| !ft_strncmp(name, "unalias", 8));
}

int	execute_builtin(t_simple_cmd *cmd, int fds[3], int *exit_code)
{
	int	ret;

	ret = 0;
	if (!cmd->words || !cmd->words->word || !cmd->words->word->word)
		return (-1);
	if (!is_builtin(cmd->words->word->word))
		return (-1);
	if (store_fds(fds))
		return (1);
	if (apply_redirections(cmd->redirects, exit_code))
	{
		restore_fds(fds);
		return (1);
	}
	ret = run_builtin(cmd);
	if (restore_fds(fds))
		return (1);
	return (ret);
}

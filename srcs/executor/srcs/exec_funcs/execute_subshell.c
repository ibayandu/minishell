/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 04:39:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "libmem.h"
#include "exec_utils.h"
#include "exec_funcs.h"
#include "executor.h"

static void	mark_commands(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == CMD_SIMPLE && cmd->value.simple)
		cmd->value.simple->flags = 1;
	else if (cmd->type == CMD_CONNECT && cmd->value.connection)
	{
		mark_commands(cmd->value.connection->first);
		mark_commands(cmd->value.connection->second);
	}
	else if (cmd->type == CMD_SUBSHELL && cmd->value.subshell)
		mark_commands(cmd->value.subshell->command);
}

int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects,
					int *exit_code)
{
	pid_t	pid;
	int		status;
	int		res;

	mark_commands(subshell->command);
	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(redirects, exit_code))
		{
			mem_free();
			exit(1);
		}
		res = execute_command(subshell->command, exit_code);
		mem_free();
		exit(res);
	}
	status = 0;
	if (!redirects || !redirects->flags)
		waitpid(pid, &status, 0);
	else
		return (pid);
	return (WEXITSTATUS(status));
}

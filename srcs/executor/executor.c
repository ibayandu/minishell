/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/14 00:50:02 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_funcs.h"
#include "executor.h"

int	execute_command(t_command *cmd, int *exit_code)
{
	if (!cmd)
		return (0);
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple(cmd->value.simple, cmd->redirects, exit_code));
	if (cmd->type == CMD_CONNECT)
		return (execute_connect(cmd->value.connection, cmd->redirects,
				exit_code));
	if (cmd->type == CMD_SUBSHELL)
		return (execute_subshell(cmd->value.subshell, cmd->redirects,
				exit_code));
	return (1);
}

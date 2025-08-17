/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/16 01:41:56 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_funcs.h"
#include "executor.h"

int	execute_command(t_command *cmd, int *exit_code)
{
	int	ret;

	ret = 0;
	if (!cmd)
		ret = 0;
	else if (cmd->type == CMD_SIMPLE)
		ret = execute_simple(cmd->value.simple, cmd->redirects, exit_code);
	else if (cmd->type == CMD_CONNECT)
		ret = execute_connect(cmd->value.connection, cmd->redirects,
				exit_code);
	else if (cmd->type == CMD_SUBSHELL)
		ret = execute_subshell(cmd->value.subshell, cmd->redirects,
				exit_code);
	else
		ret = 1;
	return (ret);
}

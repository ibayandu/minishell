/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/29 10:30:51 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "exec_utils.h"

int	execute_command(t_command *cmd, t_minishell *minishell)
{
	if (!cmd)
		return (0);
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple(cmd->value.simple, cmd->redirects, minishell));
	if (cmd->type == CMD_CONNECT)
		return (execute_connect(cmd->value.connection, minishell));
	if (cmd->type == CMD_SUBSHELL)
		return (execute_subshell(cmd->value.subshell, cmd->redirects, minishell));
	return (1);
}

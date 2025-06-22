/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:50:29 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/22 07:23:36 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "exec_utils.h"

int	execute_command(t_command *cmd)
{
	if (!cmd)
		return (0);
	if (cmd->redirects)
		apply_redirections(cmd->redirects);
	if (cmd->type == CMD_SIMPLE)
		return (execute_simple(cmd->value.simple, cmd->redirects));
	if (cmd->type == CMD_CONNECT)
		return (execute_connect(cmd->value.connection));
	if (cmd->type == CMD_SUBSHELL)
		return (execute_subshell(cmd->value.subshell, cmd->redirects));
	return (1);
}

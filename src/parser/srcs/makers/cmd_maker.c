/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_maker.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:37:01 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/20 18:58:36 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "structs.h"
#include "flags.h"
#include "collector.h"

t_command	*make_command(t_cmd_type type, t_simple_cmd *pointer)
{
	t_command	*temp;

	temp = ft_malloc(sizeof(t_command));
	if (!temp)
		return (NULL);
	temp->type = type;
	temp->value.simple = pointer;
	temp->value.simple->flags = 0;
	temp->flags = 0;
	temp->redirects = NULL;
	return (temp);
}

t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type)
{
	t_connect_cmd	*temp;

	temp = ft_malloc(sizeof(t_connect_cmd));
	if (!temp)
		return (NULL);
	temp->type = type;
	temp->first = cmd1;
	temp->second = cmd2;
	return (make_command(CMD_CONNECT, (t_simple_cmd *)temp));
}

t_command	*make_subshell_command(t_command *command)
{
	t_subshell_cmd	*temp;

	temp = ft_malloc(sizeof(t_subshell_cmd));
	temp->command = command;
	temp->flags = CMD_WANT_SUBSHELL;
	return (make_command (CMD_SUBSHELL, (t_simple_cmd *)temp));
}

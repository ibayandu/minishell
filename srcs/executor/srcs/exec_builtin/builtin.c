/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:21:11 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/07 03:29:35 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtin.h"
#include "exec_utils.h"
#include "expander.h"

int	run_builtin(t_simple_cmd *cmd)
{
	const char	*name;

	unbind_variable("_", create_tables(0));
	if (!cmd || !cmd->words || !cmd->words->word)
		return (1);
	name = cmd->words->word->word;
	if (!ft_strncmp(name, "cd", 3))
		return (builtin_cd(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "export", 7))
		return (builtin_export(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "unset", 6))
		return (builtin_unset(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "exit", 5))
		return (builtin_exit(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "echo", 5))
		return (builtin_echo(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "pwd", 4))
		return (builtin_pwd());
	else if (!ft_strncmp(name, "env", 4))
		return (builtin_env());
	else if (!ft_strncmp(name, "alias", 5))
		return (builtin_alias(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "unalias", 8))
		return (builtin_unalias(build_argv(cmd->words)));
	return (1);
}

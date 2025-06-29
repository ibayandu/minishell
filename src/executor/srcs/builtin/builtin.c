/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:21:11 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 13:12:21 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "exec_utils.h"
#include "expander.h"

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	return (!ft_strncmp(name, "cd", 3) || !ft_strncmp(name, "export", 7)
		|| !ft_strncmp(name, "unset", 6) || !ft_strncmp(name, "exit", 5)
		|| !ft_strncmp(name, "echo", 5) || !ft_strncmp(name, "env", 4)
		|| !ft_strncmp(name, "pwd", 4));
}

int	run_builtin(t_simple_cmd *cmd, t_minishell *minishell)
{
	const char	*name;

	unbind_variable("_", minishell);
	if (!cmd || !cmd->words || !cmd->words->word)
		return (1);
	name = cmd->words->word->word;
	if (!ft_strncmp(name, "cd", 3))
		return (builtin_cd(build_argv(cmd->words), minishell));
	else if (!ft_strncmp(name, "export", 7))
		return (builtin_export(build_argv(cmd->words), minishell));
	else if (!ft_strncmp(name, "unset", 6))
		return (builtin_unset(build_argv(cmd->words), minishell));
	else if (!ft_strncmp(name, "exit", 5))
		return (builtin_exit(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "echo", 5))
		return (builtin_echo(build_argv(cmd->words)));
	else if (!ft_strncmp(name, "pwd", 4))
		return (builtin_pwd());
	else if (!ft_strncmp(name, "env", 4))
		return (builtin_env(minishell));
	return (1);
}

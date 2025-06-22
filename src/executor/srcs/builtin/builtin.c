/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:21:11 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:11:15 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "exec_utils.h"

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	return (!ft_strncmp(name, "cd", 3) || !ft_strncmp(name, "export", 7)
		|| !ft_strncmp(name, "unset", 6) || !ft_strncmp(name, "exit", 5)
		|| !ft_strncmp(name, "echo", 5) || !ft_strncmp(name, "env", 4)
		|| !ft_strncmp(name, "pwd", 4));
}

int	run_builtin(t_simple_cmd *cmd)
{
	const char	*name;
	char		*errstr;

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
	errstr = ft_strjoin(name, ": builtin not implemented");
	ft_putendl_fd(errstr, 2);
	return (1);
}

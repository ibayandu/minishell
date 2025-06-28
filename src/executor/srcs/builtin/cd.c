/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:45:44 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 13:02:26 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

static int	change_directory(const char *path, t_minishell *minishell)
{
	char		cwd[PATH_MAX];
	char		*oldpwd;
	t_variable	*v;

	oldpwd = NULL;
	v = find_variable("PWD", minishell);
	if (v)
		oldpwd = v->value;
	if (chdir(path) != 0)
		return (1);
	if (oldpwd)
	{
		unbind_variable("OLDPWD", minishell);
		bind_variable("OLDPWD", oldpwd, minishell);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		unbind_variable("PWD", minishell);
		bind_variable("PWD", cwd, minishell);
	}
	else
		return (1);
	return (0);
}

int	builtin_cd(char **argv, t_minishell *minishell)
{
	const char	*path;
	char		*home;
	t_variable	*v;

	home = NULL;
	if (argv[1] == NULL)
	{
		v = find_variable("HOME", minishell);
		if (v)
			home = v->value;
		if (!home)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		path = home;
	}
	else
		path = argv[1];
	return (change_directory(path, minishell));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:45:44 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/05 09:58:55 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "exec_builtin.h"
#include "expander.h"

static void	update_directory_vars(char *oldpwd)
{
	char	*cwd;

	if (oldpwd)
	{
		unbind_variable("OLDPWD", create_tables(0));
		bind_variable("OLDPWD", oldpwd, create_tables(0));
	}
	cwd = ft_strdup(getcwd(NULL, 0));
	if (*cwd)
	{
		unbind_variable("PWD", create_tables(0));
		bind_variable("PWD", cwd, create_tables(0));
	}
}

static int	change_directory(const char *path)
{
	t_variable	*v;
	char		*oldpwd;

	oldpwd = NULL;
	v = find_variable("PWD", create_tables(0));
	if (v)
		oldpwd = v->value;
	if (chdir(path) != 0)
	{
		perror(ft_strjoin("minishell: cd: ", path));
		return (1);
	}
	update_directory_vars(oldpwd);
	return (0);
}

static int	check_cd_args(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	if (i > 2)
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	return (0);
}

static const char	*get_cd_target_path(char **argv)
{
	t_variable	*v;

	if (argv[1])
		return (argv[1]);
	v = find_variable("HOME", create_tables(0));
	if (v && v->value)
		return (v->value);
	ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
	return (NULL);
}

int	builtin_cd(char **argv)
{
	const char	*path;

	if (check_cd_args(argv))
		return (1);
	path = get_cd_target_path(argv);
	if (!path)
		return (1);
	return (change_directory(path));
}

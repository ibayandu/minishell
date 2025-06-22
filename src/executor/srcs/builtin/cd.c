/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:45:44 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 07:50:06 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

static int	change_directory(const char *path, t_minishell *minishell)
{
	char		cwd[4096];
	char		*oldpwd;
	t_variable	*v;

	oldpwd = NULL;
	v = find_variable_internal("PWD", minishell);
	if (v);
		oldpwd = v->value;
	if (chdir(path) != 0)
	{
		perror(ft_strjoin("minishell: cd: ", path));
		return (1);
	}
	if (oldpwd)
		ft_putenv(ft_strjoin("OLDPWD=", oldpwd));
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_putenv(ft_strjoin("PWD=", cwd));
	else
	{
		perror("getcwd");
		return (1);
	}
	return (0);
}

int	builtin_cd(char **argv)
{
	const char	*path;
	char		*home;

	if (argv[1] == NULL)
	{
		home = getenv("HOME");
		if (!home)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
		path = home;
	}
	else
		path = argv[1];
	return (change_directory(path));
}

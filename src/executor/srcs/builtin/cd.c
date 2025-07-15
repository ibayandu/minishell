/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:45:44 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 19:41:31 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

static void	update_directory_vars(t_minishell *minishell, char *oldpwd)
{
	char	cwd[PATH_MAX];

	if (oldpwd)
	{
		unbind_variable("OLDPWD", minishell->global_variables);
		bind_variable("OLDPWD", oldpwd, minishell->global_variables);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		unbind_variable("PWD", minishell->global_variables);
		bind_variable("PWD", cwd, minishell->global_variables);
	}
}

static int	change_directory(const char *path, t_minishell *minishell)
{
	t_variable	*v;
	char		*oldpwd;

	oldpwd = NULL;
	v = find_variable("PWD", minishell->global_variables);
	if (v)
		oldpwd = v->value;
	if (chdir(path) != 0)
	{
		perror(ft_strjoin("minishell: cd: ", path));
		return (1);
	}
	update_directory_vars(minishell, oldpwd);
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

static const char	*get_cd_target_path(char **argv, t_minishell *minishell)
{
	t_variable	*v;

	if (argv[1])
		return (argv[1]);
	v = find_variable("HOME", minishell->global_variables);
	if (v && v->value)
		return (v->value);
	ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
	return (NULL);
}

int	builtin_cd(char **argv, t_minishell *minishell)
{
	const char	*path;

	if (check_cd_args(argv))
		return (1);
	path = get_cd_target_path(argv, minishell);
	if (!path)
		return (1);
	return (change_directory(path, minishell));
}

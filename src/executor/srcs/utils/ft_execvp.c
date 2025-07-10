/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:03:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 19:09:36 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec_utils.h"
#include "expander.h"
#include <errno.h>

static char	*build_full_path(const char *dir, const char *file)
{
	size_t	file_len;
	size_t	dir_len;
	char	*full_path;

	file_len = ft_strlen(file);
	dir_len = ft_strlen(dir);
	full_path = ft_malloc(dir_len + file_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + file_len + 2);
	ft_strlcat(full_path, "/", dir_len + file_len + 2);
	ft_strlcat(full_path, file, dir_len + file_len + 2);
	return (full_path);
}

static char	*find_in_path(const char *file, t_minishell *minishell)
{
	char		*path_copy;
	char		*dir;
	char		*full_path;
	t_variable	*v;

	v = NULL;
	if (!file || !*file || ft_strchr(file, '/'))
		return (NULL);
	v = find_variable("PATH", minishell->global_variables);
	if (v && v->value)
		path_copy = ft_strdup(v->value);
	else
		path_copy = ft_strdup(".");
	dir = ft_strtok(path_copy, ":");
	while (dir)
	{
		full_path = build_full_path(dir, file);
		if (full_path && access(full_path, X_OK) == 0)
			return (full_path);
		dir = ft_strtok(NULL, ":");
	}
	return (NULL);
}

int	ft_execvp(const char *file, char *const argv[], t_minishell *minishell)
{
	char	**env;
	char		*path;
	int			result;

	if (!file || !*file)
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr(file, '/'))
	{
		unbind_variable("_", minishell->global_variables);
		bind_variable("_", (char *)file, minishell->global_variables);
		env = make_var_export_array(minishell->global_variables, 0);
		return (execve(file, argv, env));
	}
	path = find_in_path(file, minishell);
	if (path)
	{
		unbind_variable("_", minishell->global_variables);
		bind_variable("_", path, minishell->global_variables);
		env = make_var_export_array(minishell->global_variables, 0);
		result = execve(path, argv, env);
		return (result);
	}
	errno = ENOENT;
	return (-1);
}

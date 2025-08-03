/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:03:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 11:58:08 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include "libft.h"
#include "libmem.h"
#include "expander.h"
#include "exec_utils.h"

static char	*build_full_path(const char *dir, const char *file)
{
	size_t	file_len;
	size_t	dir_len;
	char	*full_path;

	file_len = ft_strlen(file);
	dir_len = ft_strlen(dir);
	full_path = mem_malloc(dir_len + file_len + 2);
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, dir, dir_len + file_len + 2);
	ft_strlcat(full_path, "/", dir_len + file_len + 2);
	ft_strlcat(full_path, file, dir_len + file_len + 2);
	return (full_path);
}

static char	*find_in_path(const char *file)
{
	char		*path_copy;
	char		*dir;
	char		*full_path;
	t_variable	*v;

	v = NULL;
	if (!file || !*file || ft_strchr(file, '/'))
		return (NULL);
	v = find_variable("PATH", create_variable_tables(0));
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

static int	exec_with_path(const char *file, char *const argv[])
{
	char	*path;
	char	**env;
	int		result;

	path = find_in_path(file);
	if (!path)
	{
		errno = ENOENT;
		return (-1);
	}
	unbind_variable("_", create_variable_tables(0));
	bind_variable("_", path, create_variable_tables(0));
	env = make_var_export_array(create_variable_tables(0), 0);
	result = execve(path, argv, env);
	return (result);
}

static int	exec_with_direct_file(const char *file, char *const argv[])
{
	char	**env;

	unbind_variable("_", create_variable_tables(0));
	bind_variable("_", (char *)file, create_variable_tables(0));
	env = make_var_export_array(create_variable_tables(0), 0);
	return (execve(file, argv, env));
}

int	ft_execvp(const char *file, char *const argv[])
{
	if (!file || !*file)
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr(file, '/'))
		return (exec_with_direct_file(file, argv));
	return (exec_with_path(file, argv));
}

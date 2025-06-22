/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execvp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 20:03:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:07:59 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "exec_utils.h"
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

static char	*find_in_path(const char *file)
{
	char	*path_env;
	char	*path_copy;
	char	*dir;
	char	*full_path;

	if (!file || !*file || ft_strchr(file, '/'))
		return (NULL);
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
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

int	ft_execvp(const char *file, char *const argv[])
{
	extern char	**environ;
	char		*path;
	int			result;

	if (!file || !*file)
	{
		errno = ENOENT;
		return (-1);
	}
	if (ft_strchr(file, '/'))
		return (execve(file, argv, environ));
	path = find_in_path(file);
	if (path)
	{
		result = execve(path, argv, environ);
		return (result);
	}
	errno = ENOENT;
	return (-1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 04:42:38 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	key_equals(const char *env_str, const char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	return (ft_strncmp(env_str, key, key_len) == 0 && env_str[key_len] == '=');
}

int	ft_unsetenv(const char *key)
{
	extern char	**environ;
	int			i;

	if (!key || ft_strchr(key, '=') != NULL)
		return (-1);
	i = 0;
	while (environ[i])
	{
		if (key_equals(environ[i], key))
		{
			while (environ[i])
			{
				environ[i] = environ[i + 1];
				i++;
			}
			return (0);
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **argv)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		if (ft_unsetenv(argv[i]) != 0)
		{
			perror("unset");
			return (1);
		}
		i++;
	}
	return (0);
}

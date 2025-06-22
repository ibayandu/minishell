/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 03:59:37 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 04:02:23 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	ft_validate_env_entry(char *env_entry, char **key_end,
		size_t *key_len)
{
	if (!env_entry || !ft_strchr(env_entry, '='))
		return (-1);
	*key_end = ft_strchr(env_entry, '=');
	*key_len = *key_end - env_entry;
	return (0);
}

static int	ft_find_and_replace_key(char *env_entry, size_t key_len)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], env_entry, key_len) == 0
			&& environ[i][key_len] == '=')
		{
			environ[i] = env_entry;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	ft_get_env_count(void)
{
	extern char	**environ;
	int			count;

	count = 0;
	while (environ[count])
		count++;
	return (count);
}

static int	ft_add_new_env_entry(char *env_entry, int env_count)
{
	extern char	**environ;
	char		**new_env;

	new_env = ft_realloc(environ, sizeof(char *) * (env_count + 2));
	if (!new_env)
		return (-1);
	environ = new_env;
	environ[env_count] = env_entry;
	environ[env_count + 1] = NULL;
	return (0);
}

int	ft_putenv(char *env_entry)
{
	char	*key_end;
	size_t	key_len;
	int		env_count;

	if (ft_validate_env_entry(env_entry, &key_end, &key_len) == -1)
		return (-1);
	if (ft_find_and_replace_key(env_entry, key_len) == 1)
		return (0);
	env_count = ft_get_env_count();
	return (ft_add_new_env_entry(env_entry, env_count));
}

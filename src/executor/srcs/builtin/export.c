/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:43:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:23:00 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	list_env(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
		printf("declare -x %s\n", environ[i++]);
}

static int	ft_export_with_value(char *arg)
{
	if (ft_putenv(ft_strdup(arg)) != 0)
	{
		perror("export");
		return (1);
	}
	return (0);
}

static char	*ft_create_env_string(char *key, char *val)
{
	size_t	len;
	char	*str;

	if (val)
	{
		len = ft_strlen(key) + ft_strlen(val) + 2;
		str = ft_malloc(len);
		if (!str)
			return (NULL);
		str = ft_strjoin(key, "=");
		str = ft_strjoin(str, val);
	}
	else
	{
		str = ft_malloc(ft_strlen(key) + 2);
		if (!str)
			return (NULL);
		str = ft_strjoin(key, "=");
	}
	return (str);
}

static int	ft_export_without_value(char *arg)
{
	char	*val;
	char	*str;

	val = getenv(arg);
	str = ft_create_env_string(arg, val);
	if (!str)
		return (1);
	ft_putenv(str);
	return (0);
}

int	builtin_export(char **argv)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_env();
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_export_with_value(argv[i]) != 0)
				return (1);
		}
		else
		{
			if (ft_export_without_value(argv[i]) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}
//todo key verilmediğinde hata vermiyor örnek olarak export =5 dediğinde çalışıyor yani saçma veya export = çalışıyor bu da saçma

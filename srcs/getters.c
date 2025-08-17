/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:24:19 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 06:47:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include "libmem.h"
#include "expander.h"
#include "prompt.h"

char	*get_short_pwd(void)
{
	char		*home;
	size_t		home_len;
	char		*short_pwd;
	t_variable	*variable;

	variable = find_variable("HOME", create_tables(0));
	if (variable)
		home = variable->value;
	else
		home = "";
	variable = find_variable("PWD", create_tables(0));
	if (!variable)
		return (ft_strdup("?"));
	home_len = ft_strlen(home);
	if (ft_strncmp(variable->value, home, home_len) == 0)
	{
		short_pwd = mem_malloc(ft_strlen(variable->value) - home_len + 2);
		if (!short_pwd)
			return (ft_strdup(variable->value));
		short_pwd[0] = '~';
		ft_strlcpy(short_pwd + 1, variable->value + home_len,
			ft_strlen(variable->value) - home_len + 2);
		return (short_pwd);
	}
	return (ft_strdup(variable->value));
}

char	*get_basename(const char *path)
{
	char	*slash;
	char	*tmp;
	char	*res;

	if (!path)
		return (ft_strdup("?"));
	slash = ft_strrchr(path, '/');
	if (!slash)
		return (ft_strdup(path));
	else if (*(slash + 1) == '\0')
	{
		tmp = ft_strndup(path, slash - path);
		res = get_basename(tmp);
		return (res);
	}
	else
		return (ft_strdup(slash + 1));
}

int	get_hostname(char *buffer, size_t size)
{
	int		fd;
	int		bytes_read;
	char	*newline;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes_read = read(fd, buffer, size - 1);
	if (bytes_read <= 0)
	{
		close(fd);
		return (-1);
	}
	buffer[bytes_read] = '\0';
	newline = ft_strchr(buffer, '\n');
	if (newline)
		*newline = '\0';
	close(fd);
	return (0);
}

char	*get_hostname_full(void)
{
	char	hostname[65];

	if (get_hostname(hostname, sizeof(hostname)) == 0)
		return (ft_strdup(hostname));
	return (ft_strdup("unknown"));
}

char	*get_username(void)
{
	t_variable	*variable;

	variable = find_variable("USER", create_tables(0));
	if (variable)
		return (ft_strdup(variable->value));
	return (ft_strdup("unknown"));
}

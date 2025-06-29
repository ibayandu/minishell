/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:45:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 03:51:41 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "builtin.h"

int	legal_option(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0 && legal_option(args[i] + 2))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i] , 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

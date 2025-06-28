/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:46:21 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 12:59:45 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_exit(char **argv)
{
	int		status;
	int		i;
	char	*p;

	status = 0;
	ft_putendl_fd("exit", STDOUT_FILENO);
	i = 0;
	while (argv[i])
		i++;
	if (i > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	if (argv[1])
	{
		p = argv[1];
		while (*p)
		{
			if (!ft_isdigit(*p))
			{
				ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: exit: ", argv[1]), ": numeric argument required"), STDERR_FILENO);
				ft_free();
				exit(2);
			}
			p++;
		}
		status = ft_atoi(argv[1]);
	}
	ft_free();
	exit(status);
}

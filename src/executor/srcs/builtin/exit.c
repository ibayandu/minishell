/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:46:21 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 03:42:56 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_exit(char **argv)
{
	int		status;
	char	*p;

	status = 0;
	if (argv[1])
	{
		p = argv[1];
		while (*p)
		{
			if (!ft_isdigit(*p))
			{
				ft_putendl_fd("exit: numeric argument required", 2);
				return (1);
			}
			p++;
		}
		status = ft_atoi(argv[1]);
	}
	exit(status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:46:21 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 01:14:17 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "libmem.h"
#include "exec_builtin.h"

static int	print_exit_and_count_args(char **argv, int is_sub)
{
	int	i;

	i = 0;
	if (!is_sub)
		ft_putendl_fd("exit", STDOUT_FILENO);
	while (argv[i])
		i++;
	return (i);
}

static int	is_numeric_argument(char *arg)
{
	int	minus;
	int	plus;
	int	i;

	minus = 0;
	plus = 0;
	i = 0;
	if (arg[0] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] == '-')
			minus++;
		else if (arg[i] == '+')
			plus++;
		else if (!ft_isdigit(arg[i]))
			return (0);
		if (minus > 1 || plus > 1)
			return (0);
		i++;
	}
	return (1);
}

static void	handle_invalid_numeric(char *arg)
{
	char	*msg;
	char	*full;

	msg = ft_strjoin("minishell: exit: ", arg);
	full = ft_strjoin(msg, ": numeric argument required");
	ft_putendl_fd(full, STDERR_FILENO);
	mem_free();
	exit(2);
}

int	builtin_exit(char **argv, int is_sub)
{
	int	argc;
	int	status;

	argc = print_exit_and_count_args(argv, is_sub);
	status = 0;
	if (argv[1])
	{
		if (!is_numeric_argument(argv[1]))
			handle_invalid_numeric(argv[1]);
		status = ft_atoi(argv[1]);
	}
	if (argc > 2)
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	mem_free();
	exit(status & 255);
}

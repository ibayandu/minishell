/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 21:34:23 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/12 20:26:42 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "alias.h"

int	builtin_alias(char **argv, t_minishell *minishell)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		list_alias(minishell);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		eq = ft_strchr(argv[i], '=');
		if (eq)
		{
			if (ft_alias_with_value(argv[i], minishell) != 0)
				return (1);
		}
		else
		{
			if (ft_alias_without_value(argv[i], minishell) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}

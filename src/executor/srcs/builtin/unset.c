/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 19:09:25 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "expander.h"

int	builtin_unset(char **argv, t_minishell *minishell)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		unbind_variable(argv[i], minishell->global_variables);
		i++;
	}
	return (0);
}

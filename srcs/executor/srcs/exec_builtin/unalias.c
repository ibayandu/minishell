/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unalias.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:31:54 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 10:08:45 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtin.h"
#include "expander.h"

int	builtin_unalias(char **argv)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		unbind_variable(argv[i], create_variable_tables(1));
		i++;
	}
	return (0);
}

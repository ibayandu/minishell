/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/05 09:59:34 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtin.h"
#include "expander.h"

int	builtin_unset(char **argv)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		unbind_variable(argv[i], create_tables(0));
		i++;
	}
	return (0);
}

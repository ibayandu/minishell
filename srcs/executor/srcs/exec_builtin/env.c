/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:02 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/05 09:59:03 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_builtin.h"
#include "expander.h"

int	builtin_env(void)
{
	char	**env;

	env = make_var_export_array(create_tables(0), 0);
	while (*env != NULL)
		ft_putendl_fd(*env++, 1);
	return (0);
}

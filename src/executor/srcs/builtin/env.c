/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 02:47:02 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 22:22:55 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "builtin.h"

int	builtin_env(t_minishell *minishell)
{
	char		**env;

	env = make_var_export_array(minishell->global_variables);
	while (*env != NULL)
		ft_putendl_fd(*env++, 1);
	return (0);
}

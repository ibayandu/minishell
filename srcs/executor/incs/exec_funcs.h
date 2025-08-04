/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_funcs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/04 13:01:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_FUNCS_H
# define EXEC_FUNCS_H

# include "parser.h"

int	execute_connect(t_connect_cmd *connect, int *exit_code);
int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, int *exit_code);
int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects,
		int *exit_code);
int	execute_builtin(t_simple_cmd *cmd, int fds[3], int *exit_code);

#endif // EXEC_FUNCS_H

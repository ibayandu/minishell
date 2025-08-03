/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_funcs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 01:15:33 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "parser_structs.h"

int	execute_connect(t_connect_cmd *connect, int *exit_code);
int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, int *exit_code);
int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects);
int	execute_builtin(t_simple_cmd *cmd, int fds[3], int *exit_code);

#endif // EXECUTE_H

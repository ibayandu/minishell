/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 18:37:28 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "executor.h"
# include "structs.h"
# include "exec_utils.h"
# include "minishell.h"
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

int	execute_connect(t_connect_cmd *connect, t_minishell *minishell);
int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, t_minishell *minishell);
int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects, t_minishell *minishell);

#endif // EXECUTE_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 08:42:58 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"
# include "structs.h"
# include "minishell.h"
# include <stdio.h>

int	is_builtin(const char *name);
int	run_builtin(t_simple_cmd *cmd, t_minishell *minishell);
int	ft_putenv(char *env_entry);

int	builtin_cd(char **argv, t_minishell *minishell);
int	builtin_export(char **args);
int	builtin_unset(char **args);
int	builtin_exit(char **args);
int	builtin_echo(char **args);
int	builtin_pwd(void);
int	builtin_env(void);

#endif // BUILTIN_H

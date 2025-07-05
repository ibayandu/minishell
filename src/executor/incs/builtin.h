/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 22:34:35 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

#include <x86_64-linux-gnu/bits/local_lim.h>
# include <stdio.h>
# include "minishell.h"
# include "structs.h"
# include "libft.h"

int	is_builtin(const char *name);
int	run_builtin(t_simple_cmd *cmd, t_minishell *minishell);
int	ft_putenv(char *env_entry);

int	builtin_cd(char **argv, t_minishell *minishell);
int	builtin_export(char **argv, t_minishell *minishell);
int	builtin_alias(char **argv, t_minishell *minishell);
int	builtin_unalias(char **argv, t_minishell *minishell);
int	builtin_unset(char **argv, t_minishell *minishell);
int	builtin_exit(char **args);
int	builtin_echo(char **args);
int	builtin_pwd(void);
int	builtin_env(t_minishell *minishell);

#endif // BUILTIN_H

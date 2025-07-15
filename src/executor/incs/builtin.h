/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 16:25:36 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"
# include "minishell.h"
# include "structs.h"
# include <stdio.h>
# include <x86_64-linux-gnu/bits/local_lim.h>

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

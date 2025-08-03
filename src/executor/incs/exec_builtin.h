/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:58 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 01:19:18 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

int	run_builtin(t_simple_cmd *cmd);
int	ft_putenv(char *env_entry);

int	builtin_cd(char **argv);
int	builtin_export(char **argv);
int	builtin_alias(char **argv);
int	builtin_unalias(char **argv);
int	builtin_unset(char **argv);
int	builtin_exit(char **args);
int	builtin_echo(char **args);
int	builtin_pwd(void);
int	builtin_env();

#endif // BUILTIN_H

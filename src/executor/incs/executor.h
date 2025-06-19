/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 14:21:03 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 14:36:25 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include "libft.h"
#include "structs.h"

// Forward declarations for executor functions
int execute_command_node(t_command *command_node, char **envp, int *original_stdin, int *original_stdout);
int execute_simple_command(t_simple_cmd *simple_cmd, char **envp);
int execute_connected_command(t_connect_cmd *connect_cmd, char **envp, int *original_stdin, int *original_stdout);
int execute_subshell_command(t_subshell_cmd *subshell_cmd, t_redirect *cmd_redirects, char **envp);

int handle_redirections(t_redirect *redirect_list, int *saved_fds, int heredoc_fds[]);
void restore_fds(int *saved_fds);
char **build_argv(t_word_list *words);
void free_argv(char **argv);
char *resolve_command_path(const char *command);
int execute_builtin(char **argv);
int execute_ast(t_command *ast_root, char ***envp_ptr);

#endif //EXECUTOR_H

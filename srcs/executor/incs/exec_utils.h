/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 00:39:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "parser.h"

int		apply_redirections(t_redirect *r, int *exit_code);
char	**build_argv(t_word_list *words);
int		ft_execvp(const char *file, char *const argv[]);
int		pipe_count_wait(t_connect_cmd *connect);
void	push_pipe(int pid, int *pids);

#endif // EXEC_UTILS_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 01:33:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

#include "parser_structs.h"

int		apply_redirections(t_redirect *r, int *exit_code);
char	**build_argv(t_word_list *words);
int		ft_execvp(const char *file, char *const argv[]);

#endif // EXEC_UTILS_H

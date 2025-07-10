/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 22:00:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "libft.h"
# include "structs.h"
# include "minishell.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

int		apply_redirections(t_redirect *r, t_minishell *minishell);
char	**build_argv(t_word_list *words);
int		ft_execvp(const char *file, char *const argv[], t_minishell *minishell);

#endif // EXEC_UTILS_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 16:25:51 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "libft.h"
# include "minishell.h"
# include "structs.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

int		apply_redirections(t_redirect *r, t_minishell *minishell);
char	**build_argv(t_word_list *words);
int		ft_execvp(const char *file, char *const argv[], t_minishell *minishell);

#endif // EXEC_UTILS_H

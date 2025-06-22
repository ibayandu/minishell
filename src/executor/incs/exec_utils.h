/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 19:05:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:09:15 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "libft.h"
# include "structs.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

void	apply_redirections(t_redirect *r);
char	**build_argv(t_word_list *words);
int		ft_execvp(const char *file, char *const argv[]);

#endif // EXEC_UTILS_H

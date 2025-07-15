/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:59:56 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/15 18:05:44 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEREDOC_H
# define FT_HEREDOC_H

# include "get_next_line.h"
# include "libft.h"
# include "makers.h"
# include "minishell.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

int		*create_fds(char **paths, t_minishell *minishell, int is_child);
pid_t	gather_here_documents_fd(char ***paths, t_minishell *minishell);

#endif // FT_HEREDOC_H

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:59:32 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/15 18:09:30 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

void	push_heredoc(t_redirect *r, t_minishell *minishell)
{
	if (minishell->need_here_doc >= HEREDOC_MAX)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded",
			STDERR_FILENO);
		ft_free();
		exit(2);
	}
	minishell->redir_stack[minishell->need_here_doc++] = r;
}

int	*create_fds(char **paths, t_minishell *minishell, int is_child)
{
	int	*fds;
	int	i;
	int	flags;

	flags = O_RDONLY;
	if (is_child)
		flags = O_CREAT | O_TRUNC | O_WRONLY;
	fds = ft_malloc(sizeof(int) * minishell->need_here_doc);
	i = -1;
	while (++i < minishell->need_here_doc)
	{
		fds[i] = open(paths[i], flags, 0644);
		if (fds[i] < 0 && is_child)
		{
			ft_free();
			exit(1);
		}
	}
	return (fds);
}

static char	**create_temps(t_minishell *minishell)
{
	int		i;
	char	**paths;

	paths = ft_malloc(sizeof(char *) * (minishell->need_here_doc + 1));
	i = -1;
	while (++i < minishell->need_here_doc)
		paths[i] = ft_strjoin("/tmp/minishell_hd_", ft_itoa(i));
	paths[i] = NULL;
	return (paths);
}

pid_t	gather_here_documents_fd(char ***paths, t_minishell *minishell)
{
	int		*fds;
	pid_t	pid;
	int		r;

	*paths = create_temps(minishell);
	pid = fork();
	if (!pid)
	{
		fds = create_fds(*paths, minishell, 1);
		r = -1;
		while (++r < minishell->need_here_doc)
		{
			make_here_document_fd(minishell->redir_stack[r], fds[r]);
			minishell->redir_stack[r] = NULL;
		}
		ft_free();
		exit(0);
	}
	return (pid);
}

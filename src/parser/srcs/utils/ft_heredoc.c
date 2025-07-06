/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:05:47 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 17:15:01 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "minishell.h"
#include "libgnl.h"
#include "makers.h"
#include "libft.h"

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

char	**create_temps(t_minishell *minishell)
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

int	ft_heredoc(t_minishell *minishell)
{
	int		status;
	pid_t	pid;
	char	**paths;
	int		*fds;
	int		r;

	status = 0;
	if (!minishell->need_here_doc)
		return (0);
	pid = gather_here_documents_fd(&paths, minishell);
	if (pid < 0)
		return (1);
	if (waitpid(pid, &status, 0) < 0)
		return (1);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		minishell->last_command_exit_value = WTERMSIG(status) + 128;
	if (!status)
	{
		fds = create_fds(paths, minishell, 0);
		r = -1;
		while (++r < minishell->need_here_doc)
		{
			if (fds[r] < 0)
				return (1);
			minishell->redir_stack[r]->redirectee->word = get_all_line(fds[r]);
			minishell->redir_stack[r] = NULL;
			unlink(paths[r]);
		}
	}
	return (status);
}

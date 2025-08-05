/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:05:47 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/05 13:38:01 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "libmem.h"
#include "libgnl.h"
#include "parser_utils.h"
#include "parser_makers.h"
#include "minishell.h"

t_redirect	**push_heredoc(t_redirect *r)
{
	static t_redirect	*redir_stack[HEREDOC_MAX + 1];
	static int			count = 0;

	if (!r)
	{
		count = 0;
		redir_stack[0] = NULL;
	}
	if (count >= HEREDOC_MAX)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded",
			STDERR_FILENO);
		mem_free();
		exit(2);
	}
	if (r)
	{
		redir_stack[count++] = r;
		redir_stack[count] = NULL;
	}
	return (redir_stack);
}

int	*create_fds(char **paths, t_redirect **redir_stack, int is_child)
{
	int	*fds;
	int	i;
	int	flags;

	flags = O_RDONLY;
	if (is_child)
		flags = O_CREAT | O_TRUNC | O_WRONLY;
	i = 0;
	while (redir_stack[i])
		i++;
	fds = mem_malloc(sizeof(int) * i);
	i = -1;
	while (redir_stack[++i])
	{
		fds[i] = open(paths[i], flags, 0644);
		if (fds[i] < 0 && is_child)
		{
			mem_free();
			exit(1);
		}
	}
	return (fds);
}

static char	**create_temps(t_redirect **redir_stack)
{
	int		i;
	char	**paths;

	i = 0;
	while (redir_stack[i])
		i++;
	paths = mem_malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (redir_stack[++i])
		paths[i] = ft_strjoin("/tmp/minishell_hd_", ft_itoa(i));
	paths[i] = NULL;
	return (paths);
}

int	read_heredoc_inputs(char **paths, t_redirect **redir_stack)
{
	int	*fds;
	int	i;

	fds = create_fds(paths, redir_stack, 0);
	i = -1;
	while (redir_stack[++i])
	{
		if (fds[i] < 0)
			return (1);
		redir_stack[i]->redirectee->word = gnl_all(fds[i]);
		redir_stack[i] = NULL;
		unlink(paths[i]);
	}
	return (0);
}

pid_t	gather_here_documents_fd(char ***paths, t_redirect **redir_stack)
{
	int		*fds;
	pid_t	pid;
	int		r;

	*paths = create_temps(redir_stack);
	pid = fork();
	if (!pid)
	{
		fds = create_fds(*paths, redir_stack, 1);
		r = -1;
		while (redir_stack[++r])
		{
			make_here_document_fd(redir_stack[r], fds[r]);
			redir_stack[r] = 0;
		}
		mem_free();
		exit(0);
	}
	return (pid);
}

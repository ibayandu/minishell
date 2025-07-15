/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 14:05:47 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/15 18:09:08 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_heredoc.h"

static int	wait_heredoc_process(pid_t pid, t_minishell *minishell)
{
	int	status;

	status = 0;
	if (waitpid(pid, &status, 0) < 0)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		minishell->last_command_exit_value = WTERMSIG(status) + 128;
	return (1);
}

static int	read_heredoc_inputs(char **paths, t_minishell *minishell)
{
	int	*fds;
	int	i;

	fds = create_fds(paths, minishell, 0);
	i = -1;
	while (++i < minishell->need_here_doc)
	{
		if (fds[i] < 0)
			return (1);
		minishell->redir_stack[i]->redirectee->word = get_all_line(fds[i]);
		minishell->redir_stack[i] = NULL;
		unlink(paths[i]);
	}
	return (0);
}

int	ft_heredoc(t_minishell *minishell)
{
	pid_t	pid;
	char	**paths;
	int		status;

	if (!minishell->need_here_doc)
		return (0);
	pid = gather_here_documents_fd(&paths, minishell);
	if (pid < 0)
		return (1);
	status = wait_heredoc_process(pid, minishell);
	if (!status)
		return (read_heredoc_inputs(paths, minishell));
	return (status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:55:25 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:31:24 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_utils.h"

static void	apply_output_redirection(t_redirect *r)
{
	int	fd;

	if (r->redir_type == REDIR_OUTPUT)
		fd = open(r->redirectee->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (r->redir_type == REDIR_APPEND)
		fd = open(r->redirectee->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return ;
	if (fd >= 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else
		perror(ft_strjoin("minishell: ", r->redirectee->word));
}

static void	apply_heredoc_redirection(t_redirect *r)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	write(pipefd[1], r->redirectee->word, ft_strlen(r->redirectee->word));
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

static void	apply_input_redirection(t_redirect *r)
{
	int	fd;

	if (r->redir_type == REDIR_INPUT)
	{
		fd = open(r->redirectee->word, O_RDONLY);
		if (fd < 0)
		{
			perror(ft_strjoin("minishell: ", r->redirectee->word));
			return ;
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (r->redir_type == REDIR_UNTIL)
		apply_heredoc_redirection(r);
}

void	apply_redirections(t_redirect *r)
{
	while (r)
	{
		if (r->redir_type == REDIR_OUTPUT || r->redir_type == REDIR_APPEND)
			apply_output_redirection(r);
		else if (r->redir_type == REDIR_INPUT || r->redir_type == REDIR_UNTIL)
			apply_input_redirection(r);
		r = r->next;
	}
}

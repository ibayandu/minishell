/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:55:25 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/29 04:00:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "exec_utils.h"
#include "expander.h"

static int	apply_output_redirection(t_redirect *r, t_minishell *minishell)
{
	int	fd;

	r->redirectee->word = redir_expand(r->redirectee, minishell);
	if (r->redirectee->word)
	{
		if (r->redir_type == REDIR_OUTPUT)
			fd = open(r->redirectee->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (r->redir_type == REDIR_APPEND)
			fd = open(r->redirectee->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return (1);
		if (fd >= 0)
		{
			dup2(fd, ft_atoi(r->redirector->word));
			close(fd);
		}
		else
		{
			perror(ft_strjoin("minishell: ", r->redirectee->word));
			return (1);
		}
	}
	else
		return (1);
	return (0);
}

static int	apply_heredoc_redirection(t_redirect *r, t_minishell *minishell)
{
	int	pipefd[2];

	r->redirectee->word = here_document_expand(r->redirectee, minishell);
	if (r->redirectee->word)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		write(pipefd[1], r->redirectee->word, ft_strlen(r->redirectee->word));
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
	}
	else
		return (1);
	return (0);
}

static int	apply_input_redirection(t_redirect *r, t_minishell *minishell)
{
	int	fd;

	if (r->redir_type == REDIR_INPUT)
	{
		r->redirectee->word = redir_expand(r->redirectee, minishell);
		if (r->redirectee->word)
		{
			fd = open(r->redirectee->word, O_RDONLY);
			if (fd < 0)
			{
				perror(ft_strjoin("minishell: ", r->redirectee->word));
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else
			return (1);
	}
	else if (r->redir_type == REDIR_UNTIL)
		if (apply_heredoc_redirection(r, minishell))
			return (1);
	return (0);
}

int	apply_redirections(t_redirect *r, t_minishell *minishell)
{
	while (r)
	{
		if (r->redir_type == REDIR_OUTPUT || r->redir_type == REDIR_APPEND)
		{
			if (apply_output_redirection(r, minishell))
				return (1);
		}
		else if (r->redir_type == REDIR_INPUT || r->redir_type == REDIR_UNTIL)
			if (apply_input_redirection(r, minishell))
				return (1);
		r = r->next;
	}
	return (0);
}

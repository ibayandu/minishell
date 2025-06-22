/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:55:25 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 18:51:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_utils.h"
#include "expander.h"

static void	apply_output_redirection(t_redirect *r, t_minishell *minishell)
{
	int	fd;

	r->redirectee->word = string_list_internal(expand_word_list(list_string(r->redirectee->word, " \t\n"), 1, minishell));
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

static void	apply_heredoc_redirection(t_redirect *r, t_minishell *minishell)
{
	int	pipefd[2];

	r->redirectee->word = string_list_internal(expand_word_list(list_string(r->redirectee->word, " \t\n"), 1, minishell));
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

static void	apply_input_redirection(t_redirect *r, t_minishell *minishell)
{
	int	fd;

	r->redirectee->word = string_list_internal(expand_word_list(list_string(r->redirectee->word, " \t\n"), 1, minishell));
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
		apply_heredoc_redirection(r, minishell);
}

void	apply_redirections(t_redirect *r, t_minishell *minishell)
{
	while (r)
	{
		if (r->redir_type == REDIR_OUTPUT || r->redir_type == REDIR_APPEND)
			apply_output_redirection(r, minishell);
		else if (r->redir_type == REDIR_INPUT || r->redir_type == REDIR_UNTIL)
			apply_input_redirection(r, minishell);
		r = r->next;
	}
}

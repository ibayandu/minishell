/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/16 23:55:49 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include "libft.h"
#include "libmem.h"
#include "parser_utils.h"
#include "parser_makers.h"
#include "minishell.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	mem_free();
	exit(130);
}

t_redirect	*make_redirection(t_word *source, t_redir_type redir_type,
				t_word *dest_and_filename, int flags)
{
	t_redirect	*temp;

	temp = mem_malloc(sizeof(t_redirect));
	temp->redirector = source;
	temp->redirectee = dest_and_filename;
	temp->here_doc_eof = NULL;
	temp->redir_type = redir_type;
	temp->flags = flags;
	temp->next = NULL;
	if (!(redir_type == REDIR_APPEND
			|| redir_type == REDIR_INPUT
			|| redir_type == REDIR_OUTPUT
			|| redir_type == REDIR_UNTIL
			|| redir_type == REDIR_IGNORE))
		return (NULL);
	return (temp);
}

static void	doc_warning(char *here_doc_eof)
{
	char	*text;

	text = "minishell: warning: here-document delimited by end of file";
	ft_putendl_fd(ft_strjoin(ft_strjoin(ft_strjoin(text, " (wanted `"),
				here_doc_eof), "')"), STDERR_FILENO);
}

void	make_here_document_fd(t_redirect *temp, int fd)
{
	char	*full_line;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	temp->here_doc_eof = string_quote_removal(temp->redirectee->word);
	if (!temp->here_doc_eof)
		return (close(fd), (void)0);
	full_line = ft_readline(PS2);
	while (full_line)
	{
		if (!*full_line)
		{
			full_line = ft_readline(PS2);
			continue ;
		}
		if (ft_strncmp(full_line, temp->here_doc_eof,
				ft_strlen(temp->here_doc_eof) + 1) == '\n')
			return (close(fd), (void)0);
		write(fd, full_line, ft_strlen(full_line));
		full_line = ft_readline(PS2);
	}
	if (!full_line)
		doc_warning(temp->here_doc_eof);
	close(fd);
}

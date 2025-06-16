/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 19:24:25 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <fcntl.h>
#include "utils.h"
#include "token.h"
#include "libft.h"

t_redirect	*make_redirection(t_word *source, t_redir_type redir_type,
				t_word *dest_and_filename)
{
	t_redirect	*temp;

	temp = ft_malloc(sizeof(t_redirect));
	temp->redirector = source;
	temp->redirectee = dest_and_filename;
	temp->here_doc_eof = 0;
	temp->redir_type = redir_type;
	temp->flags = 0;
	temp->next = NULL;
	if (redir_type == REDIR_OUTPUT)
		temp->flags = O_TRUNC | O_WRONLY | O_CREAT;
	else if (redir_type == REDIR_INPUT)
		temp->flags = O_RDONLY;
	else if (redir_type == REDIR_APPEND)
		temp->flags = O_APPEND | O_WRONLY | O_CREAT;
	else if (redir_type == REDIR_UNTIL)
		;
	else
		return (NULL);
	return (temp);
}

void	document_done(char *document, t_redirect *temp)
{
	if (!document)
		document = ft_strdup("");
	temp->redirectee->word = document;
}

void	document_warning(char *here_doc_eof)
{
	char	*text;

	text = "minishell: warning: here-document delimited by end of file";
	ft_putendl_fd(ft_strjoin(ft_strjoin(ft_strjoin(text, " (wanted `"),
				here_doc_eof), "')"), STDERR_FILENO);
}

void	make_here_document(t_redirect *temp)
{
	char	*document;
	char	*full_line;

	document = NULL;
	temp->here_doc_eof = string_quote_removal(temp->redirectee->word);
	if (!temp->here_doc_eof)
		return (temp->here_doc_eof = ft_strdup(""),
			document_done(document, temp));
	full_line = ft_readline("> ");
	while (full_line)
	{
		if (!*full_line)
		{
			full_line = ft_readline("> ");
			continue ;
		}
		if (ft_strncmp(full_line, temp->here_doc_eof,
				ft_strlen(temp->here_doc_eof) + 1) == '\n')
			return (document_done(document, temp));
		document = ft_strjoin(document, full_line);
		full_line = ft_readline("> ");
	}
	if (!full_line)
		document_warning(temp->here_doc_eof);
	document_done(document, temp);
}

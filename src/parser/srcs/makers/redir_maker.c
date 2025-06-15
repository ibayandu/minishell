/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 00:54:08 by yzeybek          ###   ########.tr       */
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

void	document_done(char *document, int document_index, t_redirect *temp)
{
	if (document)
		document[document_index] = '\0';
	else
	{
		document = ft_malloc(1);
		document[0] = '\0';
	}
	temp->redirectee->word = document;
}

void	make_here_document(t_redirect *temp)
{
	char	*document;
	char	*full_line;
	int		document_index;
	int		document_size;
	int		len;

	if (temp->redir_type != REDIR_UNTIL)
		return ;
	document = NULL;
	document_index = 0;
	document_size = 0;
	temp->here_doc_eof = string_quote_removal(temp->redirectee->word);
	if (!temp->here_doc_eof)
		return (temp->here_doc_eof = ft_strdup(""),
			document_done(document, document_index, temp));
	full_line = ft_readline("> ");
	while (full_line)
	{
		if (!*full_line)
		{
			full_line = ft_readline("> ");
			continue ;
		}
		if (ft_strncmp(full_line, temp->here_doc_eof,
				ft_strlen(temp->here_doc_eof)) == 0)
			return (document_done(document, document_index, temp));
		len = ft_strlen(full_line);
		if (len + document_index >= document_size)
		{
			if (document_size)
				document_size = 2 * (document_size + len) + 1;
			else
				document_size = len + 3;
			document = ft_realloc(document, document_size);
		}
		ft_memcpy(document + document_index, full_line, len);
		ft_memcpy(document + document_index + len, "\n", 1);
		document_index += len + 1;
		full_line = ft_readline("> ");
	}
	if (full_line == 0)
		ft_putendl_fd(ft_strjoin(ft_strjoin(
					"minishell: warning: here-document \
					delimited by end of file (wanted `",
					temp->here_doc_eof), "')"), STDERR_FILENO);
	document_done(document, document_index, temp);
}

void	gather_here_documents(t_minishell *minishell)
{
	int	r;

	r = 0;
	while (minishell->need_here_doc > 0)
	{
		make_here_document (minishell->redir_stack[r++]);
		minishell->need_here_doc--;
		minishell->redir_stack[r - 1] = 0;
	}
}

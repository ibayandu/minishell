/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 12:32:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
# include <stdio.h>
#include <fcntl.h>
#include "utils.h"
#include "token.h"
#include "libft.h"

t_redirect	*make_redirection(t_word *source, t_redir_type redir_type, t_word *dest_and_filename)
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
	else if (redir_type != REDIR_UNTIL)
		;
	else
		;// ERROR: not type match and abort
	return (temp);
}

char	*read_a_line(char	*prompt)
{
	char	*ret;

	ret = ft_absorb(readline(prompt));
	if (ret)
		add_history(ret);
	return ret;
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
	int		redir_len;
	char	*redir_word;
	char	*document;
	char	*full_line;
	int		document_index;
	int		document_size;
	int		delim_unquoted;
	char	*line;
	int		len;

	if (temp->redir_type != REDIR_UNTIL)
		// ERROR: make_here_document: bad instruction type %d
		return;
	document = NULL;
	document_index = 0;
	document_size = 0;
	redir_word = string_quote_removal(temp->redirectee->word, 0);
	if (redir_word)
		redir_len = ft_strlen(redir_word);
	else
	{
		temp->here_doc_eof = ft_malloc(1);
		temp->here_doc_eof[0] = '\0';
		return (document_done(document, document_index, temp));
	}
	temp->here_doc_eof = redir_word;
	delim_unquoted = (temp->redirectee->flags & F_QUOTED) == 0;
	full_line = read_a_line("> ");
	while (full_line)
	{
		line = full_line;
		if (*line == 0)
		{
			full_line = read_a_line("> ");
			continue;
		}
		if (ft_strncmp(line, redir_word, redir_len) == 0)
			return(document_done(document, document_index, temp));
		len = ft_strlen(line);
		if (len + document_index >= document_size)
		{
			if (document_size)
				document_size = 2 * (document_size + len) + 1;
			else
				document_size = len + 3;
			document = ft_realloc(document, document_size);
		}
		ft_memcpy(document + document_index, line, len);
		ft_memcpy(document + document_index + len, "\n", 1);
		document_index += len + 1;
		full_line = read_a_line("> ");
	}
	if (full_line == 0)
		;//ERROR: here-document delimited by end-of-file
	document_done(document, document_index, temp);
}

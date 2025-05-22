/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/22 22:37:18 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
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
	document_index = document_size = 0;
	redir_word = string_quote_removal(temp->redirectee->word, 0);
	if (redir_word)
		redir_len = ft_strlen(redir_word);
	else
	{
		temp->here_doc_eof = ft_malloc(1);
		temp->here_doc_eof[0] = '\0';
		goto document_done;
	}
	temp->here_doc_eof = redir_word;
	delim_unquoted = (temp->redirectee->flags & F_QUOTED) == 0;
	while (full_line = read_secondary_line (delim_unquoted))
	{
		line = full_line;
		if (*line == 0)
			continue;
		if (ft_strncmp(line, redir_word, redir_len) == 0 && line[redir_len] == '\n')
			goto document_done;
		len = ft_strlen(line);
		if (len + document_index >= document_size)
		{
			document_size = document_size ? 2 * (document_size + len) : len + 2;
			document = ft_realloc(document, document_size);
		}
		ft_memcpy(document + document_index, line, len);
		document_index += len;
	}
	if (full_line == 0)
		;//ERROR: here-document delimited by end-of-file
	document_done:
	if (document)
		document[document_index] = '\0';
	else
	{
		document = ft_malloc(1);
		document[0] = '\0';
	}
	temp->redirectee->word = document;
}

// void	make_here_document(t_redirect *temp)
// {
// 	write(1, "HEREDOC PARSED\n", 16);
// 	(void)temp;

// 	int		redir_len;
// 	char	*redir_word;
// 	char	*document;
// 	char	*full_line;
// 	int		document_index;
// 	int		document_size;
// 	int		delim_unquoted;
// 	char	*line;
// 	int		len;

// 	if (temp->redir_type != REDIR_UNTIL)
// 	{
// 		// ERROR: write error on stderr
// 		return ;
// 	}
// 	full_line = NULL;
// 	document = NULL;
// 	document_index = 0;
// 	document_size = 0;
// 	delim_unquoted = (temp->redirectee->flags & F_QUOTED) == 0;
// 	if (delim_unquoted == 0)
// 		redir_word = string_quote_removal(temp->redirectee->word, 0);
// 	else
// 		redir_word = ft_strdup(temp->redirectee->word);
// 	if (redir_word)
// 	{
// 		redir_len = ft_strlen(redir_word);
// 		temp->here_doc_eof = redir_word;
// 		full_line = read_secondary_line(delim_unquoted); // parse.y file function
// 		while (full_line)
// 		{
// 			line = full_line;
// 			if (*line == 0)
// 			{
// 				full_line = read_secondary_line(delim_unquoted);
// 				continue;
// 			}
// 			if (ft_strncmp(line, redir_word, redir_len) && line[redir_len] == '\n')
// 				break;
// 			if (ft_strncmp(line, redir_word, redir_len) && line[redir_len] == '\n')
// 			{
// 				shell_ungetc(line + redir_len); // parse.y file function
// 				full_line = 0;
// 				break;
// 			}
// 			len = ft_strlen(line);
// 			if (len + document_index >= document_size)
// 			{
// 				if (document_size)
// 					document_size = 2 * (document_size + len);
// 				else
// 					document_size = len + 2;
// 				document = ft_realloc(document, document_size);
// 			}
// 			ft_memcpy(document + document_index, line, len);
// 		}
// 		if (full_line == 0)
// 			// ERROR: here-document delimited by end-of-file error
// 		full_line = read_secondary_line(delim_unquoted);
// 		if (document)
// 			document[document_index] = '\0';
// 		else
// 		{
// 			document = ft_malloc(1);
// 			document[0] = '\0';
// 		}
// 		temp->redirectee->word = document;
// 	}
// 	else
// 	{
// 		temp->here_doc_eof = ft_malloc(1);
// 		temp->here_doc_eof[0] = '\0';
// 		if (document)
// 			document[document_index] = '\0';
// 		else
// 		{
// 			document = ft_malloc(1);
// 			document[0] = '\0';
// 		}
// 		temp->redirectee->word = document;
// 	}
// }


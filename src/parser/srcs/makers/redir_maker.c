/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_maker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:41:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 00:17:36 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
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
    full_line = ft_absorb( readline(">"));
    add_history(full_line);
	while (full_line)
	{
		line = full_line;
		if (*line == 0)
        {
            full_line = ft_absorb( readline(">"));
            add_history(full_line);
			continue;
        }
        if (ft_strncmp(line, redir_word, redir_len) == 0 )
			goto document_done;
		len = ft_strlen(line);
		if (len + document_index >= document_size)
		{
			document_size = document_size + len + 2;
			document = ft_realloc(document, document_size);
		}
		ft_memcpy(document + document_index, line, len);
        ft_memcpy(document+document_size+len,"\n\0",2);
		document_index += len;
        full_line = ft_absorb( readline(">"));
        add_history(full_line);
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

// static char	*read_a_line (int remove_quoted_newline)
// {
// 	static char	*line_buffer;
// 	static int 	buffer_size;
// 	int			indx;
// 	int			c;
// 	int			peekc;
// 	int			pass_next;

// 	#if defined (READLINE)
// 	if (no_line_editing)
// 	#else
// 	#endif
// 	print_prompt ();
// 	pass_next = 0;
// 	indx = 0;
// 	while (1)
// 	{
// 		QUIT;
// 		c = yy_getc (); /* Call this to get the next character of input. */
// 		if (c == 0)
// 		continue;
// 		if (c == EOF)
// 		{
// 			clearerr (stdin);
// 			if (indx == 0)
// 				return (NULL);
// 			c = '\n';
// 		}
// 		RESIZE_MALLOCED_BUFFER (line_buffer, indx, 2, buffer_size, 128);

// 		if (pass_next)
// 		{
// 			line_buffer[indx++] = c;
// 			pass_next = 0;
// 		}
// 		else if (c == '\\' && remove_quoted_newline)
// 		{
// 			QUIT;
// 			peekc = yy_getc ();
// 			if (peekc == '\n')
// 			{
// 				line_number++;
// 				continue;	/* Make the unquoted \<newline> pair disappear. */
// 			}
// 			else
// 			{
// 				yy_ungetc (peekc);
// 				pass_next = 1;
// 				line_buffer[indx++] = c;		/* Preserve the backslash. */
// 			}
// 		}
// 		else
// 		{
// 			if (remove_quoted_newline && (c == CTLESC || c == CTLNUL))
// 				line_buffer[indx++] = CTLESC;
// 			line_buffer[indx++] = c;
// 		}
// 		if (c == '\n')
// 		{
// 			line_buffer[indx] = '\0';
// 			return (line_buffer);
// 		}
// 	}
// }


// static void	prompt_again (int force)
// {
// 	char	*temp_prompt;

// 	ps1_prompt = get_string_value ("PS1");
// 	ps2_prompt = get_string_value ("PS2");
// 	ps0_prompt = get_string_value ("PS0");
// 	if (!prompt_string_pointer)
// 		prompt_string_pointer = &ps1_prompt;

// 	if (prompt_string_pointer)
// 		temp_prompt = decode_prompt_string(*prompt_string_pointer);
// 	else
// 		temp_prompt = NULL;

// 	if (temp_prompt == 0)
// 	{
// 		temp_prompt = ft_malloc(1);
// 		temp_prompt[0] = '\0';
// 	}
// 	current_prompt_string = *prompt_string_pointer;
// 	prompt_string_pointer = &ps2_prompt;
// 	if (!no_line_editing)
// 		current_readline_prompt = temp_prompt;
// 	else
// 		current_decoded_prompt = temp_prompt;
// }

// char	*read_secondary_line(int remove_quoted_newline)
// {
// 	char	*ret;
// 	int		n;
// 	int		c;

// 	prompt_string_pointer = &ps2_prompt;
// 	prompt_again (0);
// 	ret = read_a_line (remove_quoted_newline);
// 	if (ret && remember_on_history)
// 	{
// 		current_command_line_count++;
// 		maybe_add_history(ret);
// 	}
// 	return ret;
// }

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


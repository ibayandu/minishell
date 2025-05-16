/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   makers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 19:48:28 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/16 22:24:05 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parser.h"
#include "libft.h"
#include "flags.h"
#include "utils.h"

t_word	*alloc_word_desc()
{
	t_word	*temp;

	temp = ft_malloc(sizeof(t_word));
	if (!temp)
		return (NULL);
	temp->flags = 0;
	temp->word = 0;
	return (temp);
}

t_word	*make_bare_word(const char *string)
{
	t_word	*temp;

	temp = alloc_word_desc();
	if (*string)
		temp->word = ft_strdup(string);
	else
	{
		temp->word = ft_malloc(1);
		temp->word[0] = '\0';
	}
	return (temp);
}

t_word	*make_word_flags(t_word *w, const char *string)
{
	int		i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(string);
	while (i < slen)
	{
		if (string[i] == '$')
			w->flags |= W_HASDOLLAR;
		else if (string[i] == '\''
				|| string[i] == '`'
				|| string[i] == '"')
			w->flags |= W_QUOTED;
		i += ft_mbrlen(string + i, slen - i);
	}
	return (w);
}

t_word	*make_word(const char *string)
{
	t_word	*temp;

	temp = make_bare_word(string);
	return (make_word_flags(temp, string));
}

t_word	*make_word_from_token(int token)
{
	char	tokenizer[2];

	tokenizer[0] = token;
	tokenizer[1] = '\0';

	return (make_word(tokenizer));
}

t_word_list	*make_word_list(t_word *word, t_word_list *wlink)
{
	t_word_list	*temp;

	temp = ft_malloc(sizeof(t_word_list));
	if (!temp)
		return (NULL);
	temp->word = word;
	temp->next = wlink;
	return (temp);
}

t_command	*make_command(t_cmd_type type, t_simple_cmd *pointer)
{
	t_command	*temp;

	temp = ft_malloc(sizeof(t_command));
	if (!temp)
		return (NULL);
	temp->type = type;
	temp->value.simple = pointer;
	temp->value.simple->flags = 0;
	temp->flags = 0;
	temp->redirects = NULL;
	return (temp);
}

t_command	*command_connect(t_command *cmd1, t_command *cmd2, t_cnt_type type)
{
	t_connect_cmd	*temp;

	temp = ft_malloc(sizeof(t_connect_cmd));
	if (!temp)
		return (NULL);
	temp->type = type;
	temp->first = cmd1;
	temp->second = cmd2;
	return (make_command(CMD_CONNECT, (t_simple_cmd *)temp));
}
t_command	*make_bare_simple_command()
{
	t_command		*command;
	t_simple_cmd	*temp;

	command = ft_malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	temp = ft_malloc(sizeof(t_simple_cmd));
	if (!temp)
		return (NULL);
	command->value.simple = temp;
	temp->flags = 0;
	temp->words = NULL;
	temp->redirects = NULL;
	command->type = CMD_SIMPLE;
	command->redirects = NULL;
	command->flags = 0;
	return (command);
}

t_command	*make_simple_command(t_element element, t_command *command)
{
	if (command == 0)
		command = make_bare_simple_command();
	if (element.word)
		command->value.simple->words = make_word_list(element.word, command->value.simple->words);
	else if (element.redirect)
	{
		t_redirect	*r = element.redirect;
		while (r->next)
			r = r->next;
		r->next = command->value.simple->redirects;
		command->value.simple->redirects = element.redirect;
	}
	return (command);
}

void	make_here_document(t_redirect *temp)
{
	int		kill_leading;
	int		redir_len;
	char	*redir_word;
	char	*document;
	char	*full_line;
	int		document_index;
	int		document_size;
	int		delim_unquoted;

	if (temp->redir_type != REDIR_UNTIL)
	{
		// write error on stderr
		return ;
	}
	kill_leading = 0; // not <<-
	full_line = NULL;
	document = NULL;
	document_index = 0;
	document_size = 0;
	delim_unquoted = (temp->redirectee.filename->flags & W_QUOTED) == 0;
	if (delim_unquoted == 0)
		redir_word = string_quote_removal(temp->redirectee.filename->word, 0);
	else
		redir_word = ft_strdup(temp->redirectee.filename->word);


}

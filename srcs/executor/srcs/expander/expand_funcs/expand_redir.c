/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 08:48:22 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:22:58 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libmem.h"
#include "lexer.h"
#include "expander.h"
#include "parser.h"

char	*redir_expand(t_word *redir_word, int *exit_code)
{
	t_word_list	*expanded;
	t_word_list	*temp_list;
	int			expanded_something;

	expanded_something = 0;
	expanded = expand_word(redir_word, 0, &expanded_something, *exit_code);
	if (!expanded)
	{
		*exit_code = 1;
		return (NULL);
	}
	if (expanded_something)
		temp_list = word_list_split(expanded);
	else
		temp_list = expanded;
	temp_list = glob_list(temp_list);
	if ((temp_list && !temp_list->word->word) || (temp_list && temp_list->next))
	{
		*exit_code = 1;
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: ",
					redir_word->word), ": ambiguous redirect"), STDERR_FILENO);
		return (NULL);
	}
	return (string_list(temp_list));
}

static char	*add_char(char *dst, char *src, int *srcindex)
{
	int	len;

	len = ft_strlen(dst);
	dst = mem_realloc(dst, len + 2);
	dst[len] = src[*srcindex];
	*srcindex += 1;
	dst[len + 1] = '\0';
	return (dst);
}

char	*here_document_expand(t_word *document, int *exit_code)
{
	t_word_list	*expanded;
	char		*istring;
	int			sindex;

	if ((document->flags & F_QUOTED))
		return (document->word);
	sindex = 0;
	istring = ft_strdup("");
	while (document->word && document->word[sindex])
	{
		if (document->word[sindex] == '$')
			istring = ft_strjoin(istring, param_expand(document->word, &sindex,
						NULL, *exit_code)->word);
		else
			istring = add_char(istring, document->word, &sindex);
	}
	expanded = make_word_list(make_word(istring, document->flags), NULL);
	if (!expanded)
	{
		*exit_code = 1;
		return (NULL);
	}
	return (string_list(expanded));
}

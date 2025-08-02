/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 08:48:22 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 22:15:09 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "lexer.h"
#include "expander.h"

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
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: ", redir_word->word), ": ambiguous redirect"), STDERR_FILENO);
		return (NULL);
	}
	return (string_list(temp_list));
}

char	*here_document_expand(t_word *document, int *exit_code)
{
	t_word_list	*expanded;

	if ((document->flags & F_QUOTED))
		return (document->word);
	expanded = expand_word(document, 0, NULL, *exit_code);
	if (!expanded)
	{
		*exit_code = 1;
		return (NULL);
	}
	return (string_list(expanded));
}

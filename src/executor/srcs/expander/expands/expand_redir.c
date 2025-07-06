/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 08:48:22 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 20:29:53 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "expander.h"

char	*redir_expand(t_word *redir_word, t_minishell *minishell)
{
	t_word_list	*expanded;
	t_word_list	*temp_list;
	int			expanded_something;

	expanded_something = 0;
	expanded = expand_word(redir_word, 0, &expanded_something, minishell);
	if (!expanded)
	{
		minishell->last_command_exit_value = 1;
		return (NULL);
	}
	if (expanded_something)
			temp_list = word_list_split(expanded);
	else
		temp_list = expanded;
	temp_list = glob_list(temp_list);
	if ((temp_list && !temp_list->word->word) || (temp_list && temp_list->next))
	{
		minishell->last_command_exit_value = 1;
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: ", redir_word->word), ": ambiguous redirect"), STDERR_FILENO);
		return (NULL);
	}
	return (string_list(temp_list));
}

char	*here_document_expand(t_word *document, t_minishell *minishell)
{
	t_word_list	*expanded;

	if ((document->flags & F_QUOTED))
		return (document->word);
	expanded = expand_word(document, 0, NULL, minishell);
	if (!expanded)
	{
		minishell->last_command_exit_value = 1;
		return (NULL);
	}
	return (string_list(expanded));
}

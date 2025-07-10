/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:33:07 by yzeybek           #+#    #+#             */
/*   Updated: 2025/07/06 14:06:06 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "minishell.h"
#include "utils.h"



t_redirect	*parse_redirection(t_minishell *minishell)
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;

	source = NULL;
	redir_type = ft_get_redir(get_current_token());
	if (get_current_token()->token_type == T_NUMBER_LESS
		|| get_current_token()->token_type == T_NUMBER_GREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		source = make_word(ft_itoa(ft_atoi(get_current_token()->value)), 0);
	else if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
		source = make_word("1", 0);
	else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
		source = make_word("0", 0);
	if (get_next_token()->token_type == T_WORD)
		dest = make_word(get_current_token()->value,
				get_current_token()->flags);
	else
		return (ft_panic(get_current_token(), minishell), NULL);
	redirect = make_redirection(source, redir_type, dest);
	if (get_next_token() && redir_type == REDIR_UNTIL)
		push_heredoc(redirect, minishell);
	return (redirect);
}

t_redirect	*parse_redirection_list(t_minishell *minishell)
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection(minishell);
	if (!current)
		return (NULL);
	head = current;
	while (1)
	{
		if (!ft_get_redir(get_current_token()))
			break ;
		current = parse_redirection(minishell);
		if (!current)
			return (NULL);
		current->next = head;
		head = current;
	}
	return (head);
}



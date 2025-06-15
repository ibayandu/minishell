/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:33:07 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 00:48:20 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_redirect	*parse_redirection(t_minishell *minishell)
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;

	source = NULL;
	redir_type = ft_get_redir();
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
		return (ft_panic(), NULL);
	redirect = make_redirection(source, redir_type, dest);
	if (get_next_token() && redir_type == REDIR_UNTIL)
		minishell->redir_stack[minishell->need_here_doc++] = redirect;
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
		if (!ft_get_redir())
			break ;
		current = parse_redirection(minishell);
		if (!current)
			return (NULL);
		current->next = head;
		head = current;
	}
	return (head);
}

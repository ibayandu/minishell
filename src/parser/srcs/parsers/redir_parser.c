/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:33:07 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/14 02:09:32 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_redirect	*parse_redirection(void)
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;

	source = NULL;
	if (get_current_token()->token_type == T_NUMBER_LESS
		|| get_current_token()->token_type == T_NUMBER_GREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		source = make_word(ft_itoa(ft_atoi(get_current_token()->value)), 0);
	redir_type = ft_get_redir();
	get_next_token();
	if (!source)
	{
		if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
			source = make_word("1", 0);
		else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
			source = make_word("0", 0);
	}
	if (get_current_token()->token_type == T_WORD)
	{
		dest = make_word(get_current_token()->value,
				get_current_token()->flags);
		get_next_token();
	}
	else
		return (ft_panic());
	redirect = make_redirection(source, redir_type, dest);
	if (redir_type == REDIR_UNTIL)
		make_here_document(redirect);
	return (redirect);
}

t_redirect	*parse_redirection_list(void)
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection();
	head = current;
	while (1)
	{
		if (!ft_get_redir())
			break ;
		current = parse_redirection();
		current->next = head;
		head = current;
	}
	return (head);
}

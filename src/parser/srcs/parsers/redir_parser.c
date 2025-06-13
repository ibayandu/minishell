/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:33:07 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/13 02:50:21 by yzeybek          ###   ########.tr       */
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
	if (get_current_token()->token_type == T_GREAT
		|| get_current_token()->token_type == T_NUMBER_GREAT)
		redir_type = REDIR_OUTPUT;
	else if (get_current_token()->token_type == T_LESS
		|| get_current_token()->token_type == T_NUMBER_LESS)
		redir_type = REDIR_INPUT;
	else if (get_current_token()->token_type == T_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT)
		redir_type = REDIR_APPEND;
	else if (get_current_token()->token_type == T_DLESS
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		redir_type = REDIR_UNTIL;
	else
		return (NULL);
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
		if (!dest)
			return (NULL);
		get_next_token();
	}
	else
		return (ft_panic());
	redirect = make_redirection(source, redir_type, dest);
	if (!redirect)
		return (NULL);
	if (redir_type == REDIR_UNTIL)
		make_here_document(redirect);
	return (redirect);
}

t_redirect	*parse_redirection_list(void)
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection();
	if (!current)
		return (NULL);
	head = current;
	current = parse_redirection();
	while (current != NULL)
	{
		current->next = head;
		head = current;
		current = parse_redirection();
	}
	return (head);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:16:42 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 03:29:04 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_redir	*parse_redirs(t_list *tokens)
{
	t_token			*token;
	t_redir			*head;
	t_redir			*last;
	t_token_type	type;
	t_redir			*r;

	head = NULL;
	last = NULL;
	token = (t_token *)tokens->content;
	while (token && (token->token_type >= T_DLESS
			&& token->token_type <= T_LESS))
	{
		type = token->token_type;
		token = consume(tokens);
		if (!token || token->token_type != T_WORD)
			exit(EXIT_FAILURE); // HATA dosya ismi yok.
		r = ft_calloc(1, sizeof(t_redir));
		r->type = type;
		r->file = ft_strdup(consume(tokens)->value);
		if (!head)
			head = r;
		else
			last->next = r;
		last = r;
	}
	return (head);
}

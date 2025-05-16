/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 02:03:55 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 21:12:27 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "libft.h"

int	match(t_list *tokens, t_token_type type)
{
	t_token	*token;

	if (!tokens || !tokens->content)
		return (0);
	token = (t_token *)tokens->content;
	return (token->token_type == type);
}

t_token	*consume(t_list **tokens)
{
	if (*tokens)
	{
		t_token *token = (t_token *)(*tokens)->content;
		*tokens = (*tokens)->next;
		return (token);
	}
	return (NULL);
}
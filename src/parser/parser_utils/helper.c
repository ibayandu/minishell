/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 02:03:55 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 02:38:33 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "lexer.h"
#include "libft.h"

int	match(t_list *tokens, t_token_type type)
{
	t_token	*token;

	token = (t_token *)tokens->content;
	if (token && token->token_type == type)
		return (1);
	return (0);
}

t_token	*consume(t_list *tokens)
{
	t_token	*token;

	token = (t_token *)tokens->content;
	if (token)
		token = (t_token *)tokens->next->content;
	return (token);
}

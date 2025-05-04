/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 02:27:28 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 03:28:27 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_ast	*parse_sequence(t_list *tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_token	*token;
	t_ast	*node;

	token = (t_token *)tokens->content;
	left = parse_pipe(tokens);
	while (token && (token->token_type == T_AND_IF
			|| token->token_type == T_OR_IF))
	{
		token = consume(tokens);
		right = parse_pipe(tokens);
		node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_SEQUENCE;
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}

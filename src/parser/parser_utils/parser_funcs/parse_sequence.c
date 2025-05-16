/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 02:27:28 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 21:06:11 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_ast	*parse_sequence(t_list **tokens)
{
	t_ast *left = parse_pipe(tokens);
	t_token *token;

	while (*tokens && (token = (t_token *)(*tokens)->content)
		&& (token->token_type == T_AND_IF || token->token_type == T_OR_IF))
	{
		consume(tokens); // sadece ilerletiyor
		t_ast *right = parse_pipe(tokens);
		t_ast *node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_SEQUENCE;
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}
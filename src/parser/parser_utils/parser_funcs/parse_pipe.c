/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:02:18 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 21:07:24 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_ast	*parse_pipe(t_list **tokens)
{
	t_ast *left = parse_command(tokens);
	t_token *token;

	while (*tokens && (token = (t_token *)(*tokens)->content)
		&& token->token_type == T_PIPE)
	{
		consume(tokens); // '|' tokenını geç
		t_ast *right = parse_command(tokens);

		t_ast *node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_PIPE;
		node->left = left;
		node->right = right;

		left = node;
	}
	return (left);
}
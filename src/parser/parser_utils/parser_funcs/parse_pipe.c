/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:02:18 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 03:07:20 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_ast	*parse_pipe(t_list *tokens)
{
	t_ast *left = parse_command(tokens);
	while (match(tokens, T_PIPE))
	{
		tokens = tokens->next;
		t_ast *right = parse_command(tokens);
		t_ast *node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_PIPE;
		node->left = left;
		node->right = right;
		left = node;
	}
	return (left);
}
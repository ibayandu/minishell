/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:08:01 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 03:16:09 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"

t_ast	*parse_command(t_list *tokens)
{
	t_token	*token;
	t_ast	*node;
	t_ast	*node;
	char	**argv;
	int		i;

	token = (t_token *)tokens->content;
	if (match(tokens, T_LPARANTHESE))
	{
		tokens = tokens->next;
		node = ft_calloc(1, sizeof(t_ast));
		node->type = AST_SUBSHELL;
		node->left = parse_sequence(tokens);
		if (!match(tokens, T_RPARANTHESE))
			; // hata parantez kapanmadı;
		node->redirs = parse_redirs(tokens);
		return (node);
	}
	node = ft_calloc(1, sizeof(t_ast));
	node->type = AST_COMMAD;
	argv = ft_calloc(256, sizeof(char *));
	i = 0;
	while (token && token->token_type == T_WORD)
		argv[i++] = ft_strdup(consume(tokens)->value);
	node->argv = argv;
	node->redirs = parse_redirs(tokens);
	return (node);
}

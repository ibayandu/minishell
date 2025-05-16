/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:08:01 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 21:17:07 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"
#include <stdio.h>

const char	*token_type_to_string(t_token_type type)
{
	switch (type)
	{
	case T_LPARANTHESE:
		return ("T_LPARANTHESE");
	case T_RPARANTHESE:
		return ("T_RPARANTHESE");
	case T_PIPE:
		return ("T_PIPE");
	case T_AND_IF:
		return ("T_AND_IF");
	case T_OR_IF:
		return ("T_OR_IF");
	case T_WORD:
		return ("T_WORD");
	case T_LESS:
		return ("T_LESS");
	case T_GREAT:
		return ("T_GREAT");
	case T_DLESS:
		return ("T_DLESS");
	case T_DGREAT:
		return ("T_DGREAT");
	case T_EOF:
		return ("T_EOF");
	default:
		return ("UNKNOWN_TOKEN");
	}
}

t_ast	*parse_command(t_list **tokens)
{
	t_token *token;
	t_ast *node = ft_calloc(1, sizeof(t_ast));
	char **argv = ft_calloc(256, sizeof(char *));
	int i = 0;

	token = *tokens ? (t_token *)(*tokens)->content : NULL;

	if (token && token->token_type == T_LPARANTHESE)
	{
		consume(tokens); // '(' tokenını geç
		node->type = AST_SUBSHELL;
		node->left = parse_sequence(tokens);

		if (!match(*tokens, T_RPARANTHESE))
		{
			printf("!!! Beklenen: ) ama gelen: %s\n", (*tokens
					&& (*tokens)->content) ? token_type_to_string(((t_token *)(*tokens)->content)->token_type) : "NULL");
			printf("%s\n", ((t_token *)(*tokens)->content)->value);
			fprintf(stderr, "Syntax error: missing closing parenthesis\n");
			exit(EXIT_FAILURE);
		}
		consume(tokens); // ')' tokenını geç

		node->redirs = parse_redirs(tokens);
		return (node);
	}

	node->type = AST_COMMAD;

	while (*tokens && (token = (t_token *)(*tokens)->content)
		&& token->token_type == T_WORD)
	{
		argv[i++] = ft_strdup(token->value);
		consume(tokens);
	}

	node->argv = argv;
	node->redirs = parse_redirs(tokens);
	return (node);
}
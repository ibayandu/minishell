/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 03:16:42 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 21:06:37 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_funcs.h"
#include <stdio.h>

static t_redir_type	get_redir_type(t_token_type type)
{
	if (type == T_LESS)
		return (R_IN);
	else if (type == T_GREAT)
		return (R_OUT);
	else if (type == T_DLESS)
		return (R_HEREDOC);
	else if (type == T_DGREAT)
		return (R_APPEND);
	return (-1);
}

t_redir	*parse_redirs(t_list **tokens)
{
	t_token *token;
	t_redir *head = NULL;
	t_redir *last = NULL;
	t_redir *r;

	while (*tokens && (token = (t_token *)(*tokens)->content)
		&& (token->token_type == T_LESS || token->token_type == T_GREAT
			|| token->token_type == T_DLESS || token->token_type == T_DGREAT))
	{
		t_token_type type = token->token_type;
		consume(tokens); // yönlendirme operatörünü geç

		if (!*tokens || !(*tokens)->content)
		{
			fprintf(stderr,
				"Syntax error: redirection operator without file\n");
			exit(EXIT_FAILURE);
		}

		token = (t_token *)(*tokens)->content;
		if (token->token_type != T_WORD)
		{
			fprintf(stderr, "Syntax error: expected file after redirection\n");
			exit(EXIT_FAILURE);
		}

		r = ft_calloc(1, sizeof(t_redir));
		r->type = get_redir_type(type);
		r->file = ft_strdup(token->value);
		consume(tokens); // dosya adını da geç

		if (!head)
			head = r;
		else
			last->next = r;
		last = r;
	}
	return (head);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 14:26:07 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft/libft.h"
#include "includes/token.h"

t_token	*get_token(char *input)
{
	t_token	*token;

	token->token_type = T_WORD;
	token->value = ft_strdup("echo");
	return (token);
}

t_list	*lexer(char *input)
{
	t_list	*token_list;
	t_token	*token;

	token_list = ft_calloc(sizeof(t_list), 1);
	while (*input)
	{
		token = get_token(input);
		token_list->content = token;
		while (isspace(*input))
			input++;
	}
	return (token_list);
}

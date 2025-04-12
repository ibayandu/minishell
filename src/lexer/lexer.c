/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 17:57:03 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/token.h"
#include "includes/token_utils.h"

t_token_list	*lexer(char *input)
{
	t_token_list	*token_list;

	token_list = ft_calloc(sizeof(t_token_list), 1);
	token_list->count = token_count(input);
	token_list->tokens = ft_calloc(sizeof(t_list), token_list->count + 1);
	return (token_list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:02 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 01:11:41 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/lexer_utils.h"
#include "../includes/common.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static int	extract_word_without_quotes(char *input, int start, int end,
		char *output)
{
	int	j;
	int	k;

	j = 0;
	k = start;
	while (k < end)
	{
		if (input[k] != '"' && input[k] != '\'')
		{
			output[j++] = input[k];
		}
		k++;
	}
	output[j] = '\0';
	return (j);
}

void	handle_word_with_quotes(char *input, int *i, t_token *tokens,
		int *token_index)
{
	int		start;
	size_t	len;

	start = *i;
	while (input[*i] && input[*i] != '"' && input[*i] != '\'')
	{
		(*i)++;
	}
	len = *i - start;
	tokens[*token_index].value = malloc(len + 1);
	if (!tokens[*token_index].value)
		exit(ENOMEM);
	extract_word_without_quotes(input, start, *i, tokens[*token_index].value);
	tokens[*token_index].type = WORD;
	(*token_index)++;
}

void	handle_word_without_quotes(char *input, int *i, t_token *tokens,
		int *token_index)
{
	int		start;
	size_t	len;

	start = *i;
	while (input[*i] && !isspace(input[*i]) && !is_operator(&input[*i]))
	{
		(*i)++;
	}
	len = *i - start;
	tokens[*token_index].value = malloc(len + 1);
	if (!tokens[*token_index].value)
		exit(ENOMEM);
	strncpy(tokens[*token_index].value, &input[start], len);
	tokens[*token_index].value[len] = '\0';
	tokens[*token_index].type = WORD;
	(*token_index)++;
}

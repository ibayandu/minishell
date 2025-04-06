/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 05:33:46 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/common.h"
#include "includes/lexer_utils.h"
#include "includes/token_utils.h"
#include "lexer.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static t_token	*allocate_tokens(int count)
{
	t_token	*tokens;

	tokens = malloc(sizeof(t_token) * (count + 1));
	if (!tokens)
		return (NULL);
	memset(tokens, 0, sizeof(t_token) * (count + 1));
	return (tokens);
}

static void	handle_quoted_string(char *input, int *i, t_token *tokens,
		int *token_index)
{
	char	quote;
	int		start;
	size_t	len;

	quote = input[*i];
	(*i)++;
	start = *i;
	while (input[*i] != quote && input[*i] != '\0')
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	len = *i - start;
	tokens[*token_index].value = malloc(len + 1);
	if (!tokens[*token_index].value)
		exit(ENOMEM);
	strncpy(tokens[*token_index].value, &input[start], len - 1);
	tokens[*token_index].value[len] = '\0';
	tokens[*token_index].type = WORD;
	(*token_index)++;
}

static void	handle_word(char *input, int *i, t_token *tokens, int *token_index)
{
	if (input[*i] == '"' || input[*i] == '\'')
	{
		handle_word_with_quotes(input, i, tokens, token_index);
	}
	else
	{
		handle_word_without_quotes(input, i, tokens, token_index);
	}
}

static void	handle_operator(char *input, int *i, t_token *tokens,
		int *token_index)
{
	int		start;
	size_t	len;

	start = *i;
	*i += is_operator(&input[*i]);
	len = *i - start;
	tokens[*token_index].value = malloc(len + 1);
	if (!tokens[*token_index].value)
		exit(ENOMEM);
	strncpy(tokens[*token_index].value, &input[start], len);
	tokens[*token_index].value[len] = '\0';
	tokens[*token_index].type = OPERATOR;
	(*token_index)++;
	(*i)--;
}

t_tokens	*lexer(char *input)
{
	t_tokens	*ttokens;
	int			i;
	int			token_index;

	ttokens = malloc(sizeof(t_tokens));
	if (!ttokens)
		return (NULL);
	ttokens->count = token_count(input);
	ttokens->tokens = malloc(sizeof(t_token *) * ttokens->count + 1);
	if (!ttokens->tokens)
		return (NULL);
	i = 0;
	token_index = 0;
	
	ttokens->tokens = allocate_tokens(ttokens->count);
	if (!ttokens->tokens)
		return (NULL);
	while (input[i])
	{
		if (skip_whitespace_and_comments(input, &i, NULL))
			continue ;
		if (input[i] == '\'' || input[i] == '"')
			handle_quoted_string(input, &i, ttokens->tokens, &token_index);
		else if (input[i] && !is_operator(&input[i]))
			handle_word(input, &i, ttokens->tokens, &token_index);
		if (input[i] && is_operator(&input[i]))
			handle_operator(input, &i, ttokens->tokens, &token_index);
		i++;
	}
	ttokens->tokens[ttokens->count].type = ENDCMD;
	return (ttokens);
}

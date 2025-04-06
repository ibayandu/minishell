/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:37:04 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 12:47:42 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common.h"
#include <ctype.h>
#include <stdlib.h>

static void	op_handle(int *in_token, int *count, int *i, char *input)
{
	(*count)++;
	*i += is_operator(&input[*i]) - 1;
	if (in_token)
		*in_token = 0;
}

static void	quote_handle(int *count, int *i, char *input)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
	(*count)++;
}

static void	not_in_token_handle(int *in_token, int *count, int *i, char *input)
{
	if (is_operator(&input[*i]))
		op_handle(NULL, count, i, input);
	else if (input[*i] == '"' || input[*i] == '\'')
		quote_handle(count, i, input);
	else
	{
		(*count)++;
		*in_token = 1;
	}
}

int	token_count(char *input)
{
	int	i;
	int	count;
	int	in_token;

	i = 0;
	count = 0;
	in_token = 0;
	while (input[i])
	{
		if (skip_whitespace_and_comments(input, &i, &in_token))
			continue ;
		if (!in_token)
			not_in_token_handle(&in_token, &count, &i, input);
		if (in_token && is_operator(&input[i]))
			op_handle(&in_token, &count, &i, input);
		i++;
	}
	return (count);
}

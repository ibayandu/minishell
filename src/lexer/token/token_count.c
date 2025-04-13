/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 12:37:04 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/12 21:05:51 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/common.h"
#include <ctype.h>

static char	*skip_quote(char *input)
{
	char	quote;

	quote = *input;
	input++;
	while (*input && *input != quote)
		input++;
	if (*input == quote)
		input++;
	return (input);
}

static char	*token_handler(char *input, int *count)
{
	if ((*input == '\'' || *input == '"'))
	{
		input = skip_quote(input);
		(*count)++;
	}
	else if (is_operator(input))
	{
		input += is_operator(input);
		(*count)++;
	}
	else if (!isspace(*input) && *input)
	{
		(*count)++;
		while (!is_operator(input) && !isspace(*input))
			input++;
	}
	return (input);
}

int	token_count(char *input)
{
	int	count;

	count = 0;
	while (*input)
	{
		while (isspace(*input))
			input++;
		if (*input == '#')
			while (*input != '\n' && *input)
				input++;
		else
			input = token_handler(input, &count);
	}
	return (count);
}

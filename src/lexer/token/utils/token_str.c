/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 22:10:07 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_utils.h"

static int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

static void	handle_quote_open(char c, int *in_quotes, char *quote_type)
{
	*in_quotes = 1;
	*quote_type = c;
}

static void	handle_quote_close(int *in_quotes, char *quote_type)
{
	*in_quotes = 0;
	*quote_type = 0;
}

static int	should_break(char c, int in_quotes)
{
	return (ft_isspace(c) && !in_quotes);
}

char	*get_token_str(char *input)
{
	char	*result;
	char	*res_ptr;
	int		in_quotes;
	char	quote_type;

	in_quotes = 0;
	quote_type = 0;
	result = ft_calloc(ft_strlen(input) + 1, 1);
	res_ptr = result;
	while (*input)
	{
		if (is_quote(*input) && !in_quotes)
			handle_quote_open(*input, &in_quotes, &quote_type);
		else if (*input == quote_type && in_quotes)
			handle_quote_close(&in_quotes, &quote_type);
		else if (should_break(*input, in_quotes))
			break ;
		else
		{
			*res_ptr = *input;
			res_ptr++;
		}
		input++;
	}
	return (result);
}

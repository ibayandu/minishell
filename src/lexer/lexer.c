/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/21 21:52:38 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "token.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

char	*ft_strndup(const char *s1, size_t n)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	if (n > strlen)
		n = strlen;
	newstr = ft_calloc(n + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1, n);
	return (newstr);
}

static t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		ft_free();
		return (NULL);
	}
	return (token);
}

t_list	*lexer(char *input)
{
	t_list	*tokens;
	char	*start;
	char	*end;
	t_token	*token;

	tokens = NULL;
	start = input;
	while (*start)
	{
		if (ft_isspace(*start))
		{
			start++;
			continue ;
		}
		end = start;
		while (*end && !ft_isspace(*end))
			end++;
		token = create_token(T_WORD, ft_strndup(start, end - start));
		if (token)

			ft_lstadd_back(&tokens, ft_lstnew(token));
		start = end;
	}
	token = create_token(T_EOF, "");
	if (token)
		ft_lstadd_back(&tokens,ft_lstnew(token));
	return (tokens);
}

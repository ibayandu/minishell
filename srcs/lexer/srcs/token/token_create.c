/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:14 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 00:31:14 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libmem.h"
#include "common.h"
#include "token_utils.h"

/// @brief verilen parametreleri kullanarak t_token türünde bir token
/// oluşturur ve bunu geriye döner.
/// @param type token'ın hangi t_token_type türünde olduğu.
/// incs klasörü altında token.h dosyasının içinde görebilirsin.
/// @param value token'ın değeri, string türünde olmalı.
/// @return t_token* döner incs klasörü altında token.h
/// dosyasının içinde bulabilirsin.
static t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = mem_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->token_type = type;
	token->flags = get_token_flags(value);
	if (!token->value)
		return (NULL);
	return (token);
}

static char	*get_quote_str(char *token)
{
	char	*quote;

	quote = mem_calloc(sizeof(char), 2);
	while (*token)
	{
		if (*token == '"' && quote[0] != '"')
			quote[0] = '"';
		else if (*token == '\'' && quote[0] != '\'')
			quote[0] = '\'';
		token++;
	}
	return (quote);
}

static int	is_quote_closed(char *value)
{
	int		single_quote;
	int		double_quote;
	char	*errmsg;
	char	*input;

	input = value;
	single_quote = 0;
	double_quote = 0;
	while (*input)
	{
		if (*input == '"' && !single_quote)
			double_quote = !double_quote;
		else if (*input == '\'' && !double_quote)
			single_quote = !single_quote;
		input++;
	}
	if (!(single_quote == 0 && double_quote == 0))
	{
		errmsg = ft_strjoin("minishell: syntax error near unexpected token `",
				get_quote_str(value));
		errmsg = ft_strjoin(errmsg, "'");
		ft_putendl_fd(errmsg, 2);
	}
	return (single_quote == 0 && double_quote == 0);
}

/**
 * @brief kullanıcıdan alınan input'u bu fonksiyona verdiğinde senin için
 * tokenize eder.
 * @param input tokenize edilecek string.
 * @return girdiyi tiplerine göre ayrıştırarak t_list* döner.
 * @note t_token türüne incs klasörü altında token.h dosyasının
 * içinde ulaşabilirsin.
 *
 * Örnek Kullanım
 *
 * t_token	x = lexer(input);
 */
t_list	*lexer(char *input)
{
	t_list	*tokens;
	t_token	*token;

	tokens = NULL;
	while (*input)
	{
		if (ft_isspace(*input))
		{
			input++;
			continue ;
		}
		token = create_token(get_token_type(input), get_token_str(input));
		if (!is_quote_closed(token->value))
			return (NULL);
		if (token)
		{
			ft_lstadd_back(&tokens, ft_lstnew(token));
			input += ft_strlen(token->value);
		}
	}
	((t_token *)(ft_lstlast(tokens)->content))->flags = 1;
	return (tokens);
}

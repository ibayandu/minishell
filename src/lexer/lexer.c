/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 22:46:28 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "lexer.h"
#include "token_utils.h"

/**
 * @brief input'u verdiğinde ilk token'ın içinde kaç tane quote olduğunu döner.
 * @warning Açılıp kapanan quote'u 1 sayar.
 * @param input Quote sayısını öğrenmek istediğin string.
 * @return Verilen stringin içinde kaç tane quote olduğunu döner.
 *
 * Örnek Kullanım
 *
 * @code
 * int	x = quote_count(input);
 * @endcode
 */
static int	quote_count(char *input)
{
	int		count;
	char	quote;

	count = 0;
	while (*input && !ft_isspace(*input))
	{
		if (*input == '"' || *input == '\'')
		{
			quote = *input;
			input++;
			count++;
			while (*input && *input != quote)
				input++;
			if (*input == quote)
				input++;
		}
		else
			input++;
	}
	return (count);
}

/**
 * @brief kullanıcıdan alınan input'u bu fonksiyona verdiğinde senin için
 * tokenize eder.
 * @param input tokenize edilecek string.
 * @return girdiyi tiplerine göre ayrıştırarak t_token* döner.
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
		if (token)
			ft_lstadd_back(&tokens, ft_lstnew(token));
		input += ft_strlen(token->value) + (quote_count(input) * 2);
	}
	token = create_token(T_EOF, "");
	if (token)
		ft_lstadd_back(&tokens, ft_lstnew(token));
	return (tokens);
}

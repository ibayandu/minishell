/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/27 18:52:31 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "lexer.h"
#include "token_utils.h"

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
		if (token)
			ft_lstadd_back(&tokens, ft_lstnew(token));
		input += ft_strlen(token->value);
	}
	token = create_token(T_EOF, "");
	if (token)
		ft_lstadd_back(&tokens, ft_lstnew(token));
	return (tokens);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/14 02:07:10 by yzeybek          ###   ########.tr       */
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
static t_list	*lexer(char *input)
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
		{
			ft_lstadd_back(&tokens, ft_lstnew(token));
			input += ft_strlen(token->value);
		}
	}
	return (tokens);
}

/**
 * @brief Basit bir lexer başlatıcı ve iterator işlevi.
 *
 * input parametresi NULL değilse, verilen girdi stringini tokenize eder
 * ve oluşan token listesinin başlangıcını döner. input NULL ise, daha önce
 * oluşturulmuş token listesinden bir sonraki token'ı döner.
 *
 * @param input Tokenize edilecek giriş metni.
 * NULL ise mevcut listedeki sonraki token döner.
 * @return void* Token listesinde geçerli düğüm (t_list*).
 * Liste sonuna gelindiyse NULL döner.
 */
void	*init_lexer(char *input)
{
	static t_list	*tokenlist = NULL;

	if (input)
	{
		tokenlist = lexer(input);
		return (tokenlist);
	}
	return (&tokenlist);
}

/**
 * @brief Bir sonraki token'ı döner.
 *
 * Önceden başlatılmış token listesinden sıradaki token'ı alır.
 * Token listesi `init_lexer()` aracılığıyla başlatılmış olmalıdır.
 * Her çağrıda listeyi bir adım ilerletir ve geçerli token'ı döner.
 *
 * @return t_token* Geçerli token nesnesi. Token kalmamışsa NULL döner.
 */
t_token	*get_next_token(void)
{
	t_list	**token_list;

	token_list = init_lexer(NULL);
	if (!token_list)
		return (NULL);
	*token_list = (*token_list)->next;
	if (!(*token_list))
		return (NULL);
	return ((t_token *)(*token_list)->content);
}

t_token	*get_current_token(void)
{
	t_list	**token_list;

	token_list = init_lexer(NULL);
	if (!token_list)
		return (NULL);
	if (!(*token_list))
		return (NULL);
	return ((t_token *)(*token_list)->content);
}

int	consume_token(t_token_type type)
{
	if (type == get_current_token()->token_type)
	{
		get_next_token();
		return (1);
	}
	else
		return (0);
}

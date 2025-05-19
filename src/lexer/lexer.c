/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/19 18:01:02 by ibayandu         ###   ########.fr       */
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
	token = create_token(T_EOF, NULL);
	if (token)
		ft_lstadd_back(&tokens, ft_lstnew(token));
	ft_lstadd_front(&tokens, ft_lstnew(NULL));
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
	if (!tokenlist)
		return (NULL);
	tokenlist = tokenlist->next;
	return (tokenlist);
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
	t_list	*token_list;

	token_list = init_lexer(NULL);
	if (!token_list)
		return (NULL);
	return ((t_token *)token_list->content);
}

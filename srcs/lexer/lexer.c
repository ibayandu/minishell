/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/15 17:28:16 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "token_utils.h"

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
	static t_list	*tokenlist;

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
	if ((*token_list)->next)
		*token_list = (*token_list)->next;
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

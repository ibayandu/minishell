/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/11 21:22:53 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "token_utils.h"

static int	is_token_continue(char current, char before)
{
	int	is_space;

	is_space = ft_isspace(current);
	return (current && (!is_space || (is_space && before == '\\')));
}

static int	is_quote_continue(char current, char before, char quote)
{
	return (current && (current != quote || (quote == '"' && before == '\\')));
}

/// @brief bu fonksiyon verilen stringten sonraki ilk token'ın uzunluğunu döner.
/// bunu yaparken şunlara dikkat eder.
///
/// eğer verilen stringten sonraki ilk token bir operatör ise o zaman ilgili
/// operatör'ün uzunluğunu geri döner.
///
/// aksi halde
///
/// boşluk görene kadar saymaya başlar.
/// eğer bir quote görürse boşlukları umursamadan quote kapanana kadar gider.
///
/// eğer quote dışında iken bir operatör ile karşılaşırsa durur ve oraya
/// kadarki uzunluğu döner.
/// @param input ilk token'ının uzunluğunu öğrenmek istediğimiz string.
/// @return verilen parametredeki stringten sonra gelen ilk token'ın uzunluğu.
static int	token_len(char *input)
{
	int		len;
	char	quote;

	len = 0;
	if (get_token_type(input) >= 1 && get_token_type(input) <= 4)
		return (2);
	else if (get_token_type(input) != T_WORD)
		return (1);
	while (is_token_continue(input[len], input[len - 1]))
	{
		if (get_token_type(input + len) != T_WORD)
			return (len);
		else if (input[len] == '\'' || input[len] == '"')
		{
			quote = input[len++];
			while (is_quote_continue(input[len], input[len - 1], quote))
				len++;
		}
		len++;
	}
	return (len);
}

/// @brief bu fonsiyon parametresinden aldığı stringten sonraki gelen ilk
/// token'ı string olarak geri döner.
/// @param input kendisinden sonra gelen ilk token'ı dönmesini
/// istediğimiz string.
/// @return parametreden aldığı string'in ilk token'ı
char	*get_token_str(char *input)
{
	char	*token;

	token = ft_calloc(token_len(input), 1);
	return (ft_memcpy(token, input, token_len(input)));
}

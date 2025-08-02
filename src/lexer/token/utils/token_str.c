/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 19:14:46 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "common.h"
#include "token_utils.h"

/// @brief verilen char'ın quote olup olmadığını kontrol eder
/// single quote veya double quote kontrolü yapar sadece.
/// @param x kontrol edilecek olan char.
/// @return eğer girilen char single quote veya double quote ise 1
/// aksi durumda 0 döner.
static int	is_quote(char x)
{
	return (x == '\'' || x == '"');
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
	else if (get_token_type(input) == 7 || get_token_type(input) == 8)
		return (ft_nbrlen(input) + 1);
	else if (get_token_type(input) == 9 || get_token_type(input) == 10)
		return (ft_nbrlen(input) + 2);
	else if (get_token_type(input) != T_WORD)
		return (1);
	while (input[len] && !ft_isspace(input[len]))
	{
		if (get_token_type(input + len) != T_WORD && !ft_isdigit(input[len]))
			return (len);
		else if (is_quote(input[len]))
		{
			quote = input[len++];
			while (input[len] && input[len] != quote)
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

	token = mem_calloc(token_len(input) + 1, 1);
	return (ft_memcpy(token, input, token_len(input)));
}

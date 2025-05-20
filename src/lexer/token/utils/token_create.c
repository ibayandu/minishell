/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:14 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/20 20:16:26 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"
#include "token_utils.h"

/// @brief verilen parametreleri kullanarak t_token türünde bir token
/// oluşturur ve bunu geriye döner.
/// @param type token'ın hangi t_token_type türünde olduğu.
/// incs klasörü altında token.h dosyasının içinde görebilirsin.
/// @param value token'ın değeri, string türünde olmalı.
/// @return t_token* döner incs klasörü altında token.h
/// dosyasının içinde bulabilirsin.
t_token	*create_token(t_token_type type, char *value)
{
	t_token *token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->token_type = type;
	if (token->token_type != T_EOF)
		token->flags = get_token_flags(value);
	if (!token->value && type != T_EOF)
		return (NULL);
	return (token);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:14 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/27 20:59:20 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->token_type = type;
	token->value = value;
	if (!token->value)
	{
		ft_free();
		return (NULL);
	}
	return (token);
}

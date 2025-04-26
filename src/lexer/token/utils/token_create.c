/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 21:37:14 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 22:11:20 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_utils.h"

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

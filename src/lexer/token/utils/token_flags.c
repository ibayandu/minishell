/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:21:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/24 19:02:21 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int	get_token_flags(char *value)
{
	int	flags;

	flags = 0;
	while (*value)
	{
		if (*value == '$')
			flags |= F_DOLLAR;
		if (*value == '=')
			flags |= F_ASSIGNMENT;
		if (*value == '\'' || *value == '"')
		{
			flags |= F_QUOTED;
			if (*value == '"')
				flags |= F_DOUBLE_QUOTE;
			else
				flags |= F_SINGLE_QUOTE;
		}
		if (*value == '*')
			flags |= F_STAR;
		value++;
	}
	return (flags);
}

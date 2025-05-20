/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:21:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/20 19:35:53 by yzeybek          ###   ########.tr       */
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
		else if (*value == '=')
			flags |= F_ASSIGNMENT;
		else if (*value == '\'' || *value == '"')
		{
			flags |= F_QUOTED;
			if (*value == '"')
				flags |= F_DOUBLE_QUOTE;
			else
				flags |= F_SINGLE_QUOTE;
		}
		value++;
	}
	return (flags);
}

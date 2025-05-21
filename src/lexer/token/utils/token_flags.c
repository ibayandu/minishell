/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:21:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/21 22:35:58 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

int is_number(char *value)
{
	while (ft_isdigit(*value))
		value++;
	if (*value)
		return (0);
	return (1);
}

int	get_token_flags(char *value)
{
	int	flags;

	flags = 0;
	if (is_number(value))
		flags |= F_NUMBER;
	else
	{
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
	}
	return (flags);
}

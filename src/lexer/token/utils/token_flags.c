/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_flags.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:21:39 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/19 20:51:09 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

int	get_token_flags(char *value)
{
	int	flags;

	flags = 0;
	while (*value)
	{
		if (*value == '$')
			flags |= DOLLAR;
		else if (*value == '=')
			flags |= ASSIGNMENT;
		else if (*value == '\'' || *value == '"')
		{
			flags |= QUOTED;
			if (*value == '"')
				flags |= DOUBLE_QUOTE;
			else
				flags |= SINGLE_QUOTE;
		}
		value++;
	}
	return (flags);
}

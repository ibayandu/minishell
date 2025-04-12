/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:51:38 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 17:53:54 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft/libft.h"

int	is_operator(char *str)
{
	int			i;
	const char	*op_keywords[] = {"<<-", "&&", "||", "<<", ">>", "<&", ">&",
		"<>", ">|", "{", "}", "!", "|", "(", ")", "<", ">", "&", ";", 0};

	i = -1;
	while (i++ < 18)
		if (ft_strncmp(str, op_keywords[i], ft_strlen(op_keywords[i])) == 0)
			return (ft_strlen(op_keywords[i]));
	return (0);
}

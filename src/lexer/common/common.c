/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:51:38 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/19 20:52:22 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

t_token_type	is_number(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			break ;
	}
	if (ft_strncmp(str + i, "<<", 2) == 0)
		return (T_NUMBER_DLESS);
	else if (ft_strncmp(str + i, ">>", 2) == 0)
		return (T_NUMBER_DGREAT);
	else if (ft_strncmp(str + i, "<", 1) == 0)
		return (T_NUMBER_LESS);
	else if (ft_strncmp(str + i, ">", 1) == 0)
		return (T_NUMBER_GREAT);
	else
		return (0);
}

/// @brief verilen string'ten sonraki ilk token'ın türünü geriye döner.
/// @param str tipini öğrenmek istediğim string.
/// @return t_token_type türünde değer döner incs klasörü altında token.h
/// içerisinde ilgili enum'ı bulabilirsin.
t_token_type	get_token_type(char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0)
		return (T_DLESS);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (T_DGREAT);
	else if (ft_strncmp(str, "||", 2) == 0)
		return (T_OR_IF);
	else if (ft_strncmp(str, "&&", 2) == 0)
		return (T_AND_IF);
	else if (ft_strncmp(str, "<", 1) == 0)
		return (T_LESS);
	else if (ft_strncmp(str, ">", 1) == 0)
		return (T_GREAT);
	else if (ft_strncmp(str, "|", 1) == 0)
		return (T_PIPE);
	else if (ft_strncmp(str, "(", 1) == 0)
		return (T_LPARANTHESE);
	else if (ft_strncmp(str, ")", 1) == 0)
		return (T_RPARANTHESE);
	else if (ft_strncmp(str, "\n", 1) == 0)
		return (T_NL);
	else if (ft_strncmp(str, "\n", 1) == 0)
		return (T_NL);
	else if (is_number(str))
		return (is_number(str));
	return (T_WORD);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:51:38 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/26 18:48:01 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "token.h"

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
	else if (ft_strncmp(str, "'", 1) == 0)
		return (T_SQUOTE);
	else if (ft_strncmp(str, "\"", 1) == 0)
		return (T_DQUOTE);
	else if (ft_strncmp(str, "(", 1) == 0)
		return (T_LPARANTHESE);
	else if (ft_strncmp(str, ")", 1) == 0)
		return (T_RPARANTHESE);
	return (T_WORD);
}

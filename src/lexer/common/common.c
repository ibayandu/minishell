/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:51:38 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 14:36:29 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft/libft.h"
#include "includes/token.h"

int	is_operator(char *str)
{
	int			i;
	const int	op_keywords[11][3] = {{'<', '<', T_DLESS}, {'>', '>', T_DGREAT},
			{'<', 0, T_LESS}, {'>', 0, T_GREAT}, {'\'', 0, T_SQUOTE}, {'"', 0,
			T_DQUOTE}, {'|', 0, T_PIPE}, {'&', '&', T_AND_IF}, {'|', '|',
			T_OR_IF}, {'(', 0, T_LPARANTHESE}, {')', 0, T_RPARANTHESE}};

	i = -1;
	while (i++ < 18)
		if (ft_strncmp(str, op_keywords[i], ft_strlen(op_keywords[i])) == 0)
			return (ft_strlen(op_keywords[i]));
	return (0);
}

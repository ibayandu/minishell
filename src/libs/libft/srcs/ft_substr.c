/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:42:32 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 19:52:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

char	*ft_substr(t_memblock **head, char const *s, unsigned int start,
		size_t len)
{
	char	*rtn;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(head, ""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	i = 0;
	rtn = ft_calloc(head, len + 1, sizeof(char));
	if (!rtn)
		return (NULL);
	while (i < len)
		rtn[i++] = s[start++];
	return (rtn);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:42:32 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/14 21:18:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*rtn;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) < start)
		return (ft_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	i = 0;
	rtn = ft_calloc(len + 1, sizeof(char));
	if (!rtn)
		return (NULL);
	while (i < len)
		rtn[i++] = s[start++];
	return (rtn);
}

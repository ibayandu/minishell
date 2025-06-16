/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:47:08 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/16 14:39:48 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	s1len;
	size_t	s2len;
	size_t	total_len;

	if (!s2)
		return (NULL);
	if (!s1)
		s1 = ft_strdup("");
	s2len = ft_strlen(s2);
	s1len = ft_strlen(s1);
	total_len = s1len + s2len;
	result = (char *)ft_malloc((total_len * sizeof(char)) + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, s1len + 1);
	ft_strlcpy((result + s1len), s2, s2len + 1);
	return (result);
}

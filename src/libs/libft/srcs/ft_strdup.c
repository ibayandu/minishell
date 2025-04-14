/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:32:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/14 21:13:33 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

char	*ft_strdup(const char *s1)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	newstr = ft_calloc(strlen + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1, strlen);
	return (newstr);
}

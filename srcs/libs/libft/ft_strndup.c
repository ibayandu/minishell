/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:20:27 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 19:17:40 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	if (n > strlen)
		n = strlen;
	newstr = mem_calloc(n + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1, n);
	return (newstr);
}

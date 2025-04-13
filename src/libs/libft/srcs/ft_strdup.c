/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:32:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 19:48:48 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

char	*ft_strdup(t_memblock **head, const char *s1)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	newstr = ft_calloc(head, strlen + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1, strlen);
	return (newstr);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 10:32:54 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/02 20:07:42 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	newstr = mem_calloc(strlen + 1, sizeof(char));
	ft_memcpy(newstr, s1, strlen);
	return (newstr);
}

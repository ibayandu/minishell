/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:20:27 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/19 14:21:08 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*newstr;
	size_t	strlen;

	strlen = ft_strlen(s1);
	if (n > strlen)
		n = strlen;
	newstr = ft_calloc(n + 1, sizeof(char));
	if (!newstr)
		return (NULL);
	ft_memcpy(newstr, s1, n);
	return (newstr);
}

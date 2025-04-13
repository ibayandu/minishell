/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:30:33 by ibayandu          #+#    #+#             */
/*   Updated: 2024/10/12 10:21:14 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dest;
	char const	*source;

	if (!dst && !src)
		return (0);
	dest = dst;
	source = src;
	if (source < dest)
	{
		while (len-- > 0)
		{
			*(dest + len) = *(source + len);
		}
	}
	else
		while (len-- > 0)
			*dest++ = *source++;
	return (dst);
}

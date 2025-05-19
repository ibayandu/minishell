/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:22:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/04 01:46:00 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *destaddr, const void *srcaddr, size_t len)
{
	unsigned char	*dest;
	unsigned char	*src;

	if (!destaddr && !srcaddr)
		return (0);
	src = (unsigned char *)srcaddr;
	dest = (unsigned char *)destaddr;
	while (len-- > 0)
		*dest++ = *src++;
	return (destaddr);
}

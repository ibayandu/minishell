/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 09:22:20 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/13 19:46:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/libft.h"

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

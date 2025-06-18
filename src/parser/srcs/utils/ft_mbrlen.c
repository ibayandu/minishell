/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mbrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:38:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/17 13:15:46 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_mbrlen(const char *s, size_t max_len)
{
	const unsigned char	c = (unsigned char)s[0];

	if (!s || max_len == 0)
		return (0);
	if (c < 0x80)
		return (1);
	else if ((c >> 5) == 0x6 && max_len >= 2)
		return (2);
	else if ((c >> 4) == 0xE && max_len >= 3)
		return (3);
	else if ((c >> 3) == 0x1E && max_len >= 4)
		return (4);
	return (1);
}

size_t	add_mbqchar_body(char **dst, const char *src, size_t *si, size_t srcsize)
{
	size_t	mblen;
	size_t	start;

	mblen = 1;
	start = *si;
	if ((unsigned char)src[start] < 0x80)
		mblen = src[start] != '\0' ? 1 : 0;
	else
	{
		mblen = ft_mbrlen(src + start, srcsize - start);
		if (mblen < 1)
			mblen = 1;
	}
	*dst = ft_malloc(mblen + 2);
	(*dst)[0] = '\001';
	ft_memcpy(*dst + 1, src + start, mblen);
	(*dst)[mblen + 1] = '\0';
	*si = start + mblen;
	return (mblen + 2);
}

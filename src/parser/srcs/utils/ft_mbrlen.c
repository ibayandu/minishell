/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mbrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:38:24 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/16 20:42:01 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_mbrlen(const char *s, size_t max_len)
{
	const unsigned char c = (unsigned char)s[0];

	if (!s || max_len == 0)
		return 0;
	if (c < 0x80)
		return 1;
	else if ((c >> 5) == 0x6 && max_len >= 2)
		return 2;
	else if ((c >> 4) == 0xE && max_len >= 3)
		return 3;
	else if ((c >> 3) == 0x1E && max_len >= 4)
		return 4;
	return 1;
}

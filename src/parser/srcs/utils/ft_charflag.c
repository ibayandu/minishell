/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_charflag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 23:37:00 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/16 23:55:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "flags.h"
#include "libft.h"

static int	ft_charflags(unsigned char c)
{
	int f;

	f = 0;
	if (c == '$' || c == '"' || c == '\\' || c == '`')
		f |= CBSDQUOTE;
	return f;
}

int	ft_charflag(char c, int flag)
{
	int	flags;
	flags = ft_charflags((unsigned char)c);
	return ((flags & flag) != 0);
}

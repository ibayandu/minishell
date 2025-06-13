/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/13 02:08:18 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include "flags.h"

char	*string_extract_single_quoted(char *string, int *sindex, int allowesc)
{
	int		i;
	size_t	slen;
	char	*t;
	int		pass_next;

	if (MB_CUR_MAX > 1)
		slen = ft_strlen(string + *sindex) + *sindex;
	else
		slen = 0;
	i = *sindex;
	pass_next = 0;
	while (string[i])
	{
		if (pass_next)
		{
			pass_next = 0;
			i += ft_mbrlen(string + i, slen - i);
			continue ;
		}
		if (allowesc && string[i] == '\\')
			pass_next++;
		else if (string[i] == '\'')
			break ;
		i += ft_mbrlen(string + i, slen - i);
	}
	t = ft_substr(string, *sindex, i);
	if (string[i])
		i++;
	*sindex = i;
	return (t);
}

char	*string_quote_removal(char *string)
{
	size_t			slen;
	char			*r;
	char			*result_string;
	char			*temp;
	int				sindex;
	int				tindex;
	int				dquote;
	unsigned char	c;

	slen = ft_strlen(string);
	result_string = ft_malloc(slen + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	c = string[sindex];
	while (c)
	{
		if (c == '\\')
		{
			c = string[++sindex];
			if (c == 0)
			{
				*r++ = '\\';
				continue ;
			}
			if (dquote && ft_charflag(c, CBSDQUOTE))
				*r++ = '\\';
			*(r)++ = string[sindex];
			sindex++;
		}
		else if (c == '\'')
		{
			if (dquote)
			{
				*r++ = c;
				sindex++;
				continue ;
			}
			tindex = sindex + 1;
			temp = string_extract_single_quoted(string, &tindex, 0);
			if (temp)
			{
				ft_strlcpy(r, temp, ft_strlen(temp));
				r += ft_strlen(r);
			}
			sindex = tindex;
		}
		else if (c == '"')
		{
			dquote = 1 - dquote;
			sindex++;
		}
		else
		{
			*(r)++ = string[sindex];
			sindex++;
		}
		c = string[sindex];
	}
	*r = '\0';
	return (result_string);
}

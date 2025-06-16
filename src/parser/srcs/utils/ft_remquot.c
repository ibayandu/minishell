/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 15:50:10 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include "flags.h"

char	*string_extract_single_quoted(char *string, int *sindex)
{
	int		i;
	size_t	slen;
	char	*t;

	if (MB_CUR_MAX > 1)
		slen = ft_strlen(string + *sindex) + *sindex;
	else
		slen = 0;
	i = *sindex;
	while (string[i])
	{
		if (string[i] == '\'')
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
	char			*r;
	char			*result_string;
	char			*temp;
	int				sindex;
	int				tindex;
	int				dquote;
	unsigned char	c;

	result_string = ft_malloc(ft_strlen(string) + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	c = string[sindex];
	while (c)
	{
		if (c == '\'')
		{
			if (dquote)
			{
				*r++ = c;
				sindex++;
				continue ;
			}
			tindex = sindex + 1;
			temp = string_extract_single_quoted(string, &tindex);
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

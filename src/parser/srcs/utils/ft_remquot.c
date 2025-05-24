/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/24 21:58:40 by ibayandu         ###   ########.fr       */
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

	if (MB_CUR_MAX >  1)
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
			continue;
		}
		if (allowesc && string[i] == '\\')
			pass_next++;
		else if (string[i] == '\'')
			break;
		i += ft_mbrlen(string + i, slen - i);
	}
	t = ft_substr(string, *sindex, i);
	if (string[i])
		i++;
	*sindex = i;
	return (t);
}

char	*string_quote_removal(char *string, int quoted)
{
	size_t			slen;
	char			*r;
	char			*result_string;
	char			*temp;
	char			*send;
	int				sindex;
	int				tindex;
	int				dquote;
	unsigned char	c;

	slen = ft_strlen(string);
	send = string + slen;
	result_string = ft_malloc(slen + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	while (string[sindex])
	{
		c = string[sindex];
		if (c == '\\')
		{
			c = string[++sindex];
			if (c == 0)
			{
				*r++ = '\\';
			}
			else if (((quoted & (Q_HERE_DOCUMENT | Q_DOUBLE_QUOTES)) || dquote) && ft_charflag(c, CBSDQUOTE))
				*r++ = '\\';
		}
		else if (c == '\'')
		{
			if ((quoted & (Q_DOUBLE_QUOTES | Q_DOUBLE_QUOTES)) || dquote)
			{
				*r++ = c;
				sindex++;
			}
			else
			{
				tindex = sindex + 1;
				temp = string_extract_single_quoted(string, &tindex, 0);
				if (temp)
				{
					ft_strlcpy(r, temp, ft_strlen(temp));
					r += ft_strlen(r);
				}
				sindex = tindex;
			}
		}
		else if (c == '"')
		{
			dquote = 1 - dquote;
			sindex++;
		}
		else
		{
			*r++ = string[sindex];
			sindex++;
		}
	}
	*r = '\0';
	return (result_string);
}

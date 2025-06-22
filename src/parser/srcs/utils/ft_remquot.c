/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/21 21:01:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

char	*string_extract_single_quoted(char *string, int *sindex)
{
	int		i;
	size_t	slen;
	char	*t;

	(*sindex)++;
	slen = 0;
	i = *sindex;
	while (string[i])
	{
		if (string[i] == '\'')
			break ;
		i++;
	}
	t = ft_substr(string, *sindex, i);
	if (string[i])
		i++;
	*sindex = i;
	return (t);
}

char	*string_quote_removal(char *string)
{
	char	*r;
	char	*result_string;
	int		sindex;
	int		dquote;
	char	*temp;

	result_string = ft_malloc(ft_strlen(string) + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	temp = NULL;
	while (string[sindex])
	{
		if (string[sindex] == '\'' && !dquote)
		{
			temp = string_extract_single_quoted(string, &sindex);
			if (temp)
			{
				ft_strlcpy(r, temp, ft_strlen(temp));
				r += ft_strlen(r);
			}
		}
		else if (string[sindex] == '"')
			dquote = 1 - dquote + (0 * sindex++);
		else
			*r++ = string[sindex++];
	}
	*r = '\0';
	return (result_string);
}

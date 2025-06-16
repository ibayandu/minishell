/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 18:45:21 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

void	string_extract_single_quoted(char *string, int *sindex, char **r)
{
	int		i;
	size_t	slen;
	char	*t;

	(*sindex)++;
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
	if (t)
	{
		ft_strlcpy(*r, t, ft_strlen(t));
		*r += ft_strlen(*r);
	}
}

char	*string_quote_removal(char *string)
{
	char	*r;
	char	*result_string;
	int		sindex;
	int		dquote;

	result_string = ft_malloc(ft_strlen(string) + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	while (string[sindex])
	{
		if (string[sindex] == '\'' && !dquote)
			string_extract_single_quoted(string, &sindex, &r);
		else if (string[sindex] == '"')
			dquote = 1 - dquote + (0 * sindex++);
		else
			*r++ = string[sindex++];
	}
	*r = '\0';
	return (result_string);
}

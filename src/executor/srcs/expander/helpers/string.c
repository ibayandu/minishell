/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 23:20:56 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/19 21:12:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

char	*string_extract_verbatim(char *string, size_t slen, size_t *sindex, const char *charlist)
{
	char	*temp;
	int		i;

	if (charlist[0] == '\'' && charlist[1] == '\0')
	{
		string_extract_single_quoted(string, sindex, &temp);
		(*sindex)--;
		return (temp);
	}
	if (!*charlist)
	{
		temp = ft_substr(string, *sindex, slen);
		*sindex = slen;
		return (temp);
	}
	i = *sindex;
	while (i < (int)slen && string[i])
	{
		if (ft_strchr(charlist, string[i]))
			break ;
		i++;
	}
	temp = ft_substr(string, *sindex, i);
	*sindex = i;
	return (temp);
}

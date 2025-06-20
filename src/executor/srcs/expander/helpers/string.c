/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 23:20:56 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 12:55:29 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"

char	*string_extract_verbatim(char *string, size_t slen, int *sindex, char *charlist)
{
	int		i;
	int		c;
	char	*temp;

	if (charlist[0] == '\'' && charlist[1] == '\0')
	{
		string_extract_single_quoted(string, sindex, &temp);
		--*sindex;
		return temp;
	}
	if (!*charlist)
	{
		temp = string + *sindex;
		c = (*sindex == 0) ? slen : ft_strlen(temp);
		temp = ft_strdup(temp);
		*sindex += c;
		return (temp);
	}
	i = *sindex;
	while ((c = string[i]))
	{
		if (ft_strchr(charlist, c))
			break;
		i++;
	}
	temp = ft_substr(string, *sindex, i - *sindex);
	*sindex = i;
	return (temp);
}

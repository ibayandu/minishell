/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remquot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:20:04 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/07 05:39:35 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libmem.h"
#include "parser_utils.h"

char	*string_extract_single_quoted(char *string, int *sindex)
{
	int		i;
	char	*t;

	(*sindex)++;
	i = *sindex;
	while (string[i])
	{
		if (string[i] == '\'')
			break ;
		i++;
	}
	t = ft_substr(string, *sindex, i - *sindex);
	if (string[i])
		i++;
	*sindex = i;
	return (t);
}

static void	handle_single_quote(char *string, int *sindex, char **r)
{
	char	*temp;

	temp = string_extract_single_quoted(string, sindex);
	if (temp)
	{
		ft_strlcpy(*r, temp, ft_strlen(temp) + 1);
		*r += ft_strlen(temp);
	}
}

static void	handle_double_quote(int *dquote, int *sindex)
{
	*dquote = 1 - *dquote;
	(*sindex)++;
}

char	*string_quote_removal(char *string)
{
	char	*r;
	char	*result_string;
	int		sindex;
	int		dquote;

	result_string = mem_malloc(ft_strlen(string) + 1);
	r = result_string;
	dquote = 0;
	sindex = 0;
	while (string[sindex])
	{
		if (string[sindex] == '\'' && !dquote)
			handle_single_quote(string, &sindex, &r);
		else if (string[sindex] == '"')
			handle_double_quote(&dquote, &sindex);
		else
			*r++ = string[sindex++];
	}
	*r = '\0';
	return (result_string);
}

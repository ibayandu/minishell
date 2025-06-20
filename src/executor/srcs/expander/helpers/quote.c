/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 20:40:08 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 06:23:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "structs.h"

char	*dequote_string(char *string)
{
	char	*s;
	char	*t;
	size_t	slen;
	char	*result;
	char	*send;

	slen = ft_strlen(string);
	t = result = ft_malloc(slen + 1);
	send = string + slen;
	s = string;
	while (*s)
	{
		if (*s == CTLESC)
		{
			s++;
			if (*s == '\0')
				break;
		}
		COPY_CHAR_P (t, s, send);
	}
	*t = '\0';
	return (result);
}

t_word_list	*dequote_list (t_word_list *list)
{
	char		*s;
	t_word_list	*tlist;

	tlist = list;
	while (tlist)
	{
		s = dequote_string(tlist->word->word);
		tlist->word->word = s;
		tlist = tlist->next;
	}
	return (list);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_maker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:38:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/19 15:37:17 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"
#include "libft.h"
#include "flags.h"

t_word	*alloc_word_desc()
{
	t_word	*temp;

	temp = ft_malloc(sizeof(t_word));
	if (!temp)
		return (NULL);
	temp->flags = 0;
	temp->word = 0;
	return (temp);
}

t_word	*make_bare_word(const char *string)
{
	t_word	*temp;

	temp = alloc_word_desc();
	if (*string)
		temp->word = ft_strdup(string);
	else
	{
		temp->word = ft_malloc(1);
		temp->word[0] = '\0';
	}
	return (temp);
}

t_word	*make_word_flags(t_word *w, const char *string)
{
	int		i;
	size_t	slen;

	i = 0;
	slen = ft_strlen(string);
	while (i < slen)
	{
		if (string[i] == '$')
			w->flags |= W_HASDOLLAR;
		else if (string[i] == '\''
				|| string[i] == '"')
			w->flags |= W_QUOTED;
		i += ft_mbrlen(string + i, slen - i);
	}
	return (w);
}

t_word	*make_word(const char *string)
{
	t_word	*temp;

	temp = make_bare_word(string);
	return (make_word_flags(temp, string));
}

t_word	*make_word_from_token(int token)
{
	char	tokenizer[2];

	tokenizer[0] = token;
	tokenizer[1] = '\0';

	return (make_word(tokenizer));
}

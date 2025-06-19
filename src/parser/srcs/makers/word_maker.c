/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_maker.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:38:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 18:26:44 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"
#include "libft.h"

t_word	*alloc_word_desc(void)
{
	t_word	*temp;

	temp = ft_malloc(sizeof(t_word));
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

t_word	*make_word_flags(t_word *w, const char flags)
{
	w->flags = flags;
	return (w);
}

t_word	*make_word(const char *string, const char flags)
{
	t_word	*temp;

	temp = make_bare_word(string);
	return (make_word_flags(temp, flags));
}

t_word	*make_word_from_token(int token)
{
	char	tokenizer[2];

	tokenizer[0] = token;
	tokenizer[1] = '\0';
	return (make_word(tokenizer, 0));
}

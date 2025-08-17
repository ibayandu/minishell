/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:59 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:21:25 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static char	*add_char(char *dst, char *src, int *srcindex, int quoted)
{
	int	len;

	if (src[*srcindex] == '*' && !quoted)
		src[*srcindex] = CTLESC;
	len = ft_strlen(dst);
	dst = mem_realloc(dst, len + 2);
	dst[len] = src[*srcindex];
	*srcindex += 1;
	dst[len + 1] = '\0';
	return (dst);
}

static void	decide_flags(char *str, int *flags)
{
	if (str && *str && !ft_strchr(str, '*'))
		*flags &= ~F_QUOTED;
	else if (str && *str)
		*flags |= F_QUOTED;
}

static char	*handle_quotes(t_word *word, int *sindex, int exit_code, int quoted)
{
	t_word_list	*list;
	char		*temp;

	if (word->word[*sindex] == '"')
	{
		temp = string_extract_double_quoted(word->word, sindex);
		decide_flags(temp, &word->flags);
		list = NULL;
		if (temp && *temp)
			list = expand_word(make_bare_word(temp), 1, NULL, exit_code);
		if (list && list->next)
			return (string_list(list));
		else if (list)
			return (ft_strdup(list->word->word));
	}
	else if (word->word[*sindex] == '\'' && !quoted)
	{
		temp = string_extract_single_quoted(word->word, sindex);
		decide_flags(temp, &word->flags);
		if (temp && *temp)
			return (temp);
	}
	return (NULL);
}

t_word_list	*expand_word(t_word *word, int quoted, int *is_expand,
	int exit_code)
{
	char		*istring;
	int			sindex;

	sindex = 0;
	istring = ft_strdup("");
	while (word->word && word->word[sindex])
	{
		if (word->word[sindex] == '$')
			istring = ft_strjoin(istring, param_expand(word->word, &sindex,
						is_expand, exit_code)->word);
		else if (word->word[sindex] == '"'
			|| (word->word[sindex] == '\'' && !quoted))
			istring = ft_strjoin(istring,
					handle_quotes(word, &sindex, exit_code, quoted));
		else
			istring = add_char(istring, word->word, &sindex, quoted);
	}
	return (make_word_list(make_word(istring, word->flags), NULL));
}

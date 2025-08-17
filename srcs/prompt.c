/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 23:00:00 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/17 06:46:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libmem.h"
#include "expander.h"
#include "minishell.h"
#include "prompt.h"

static char	*add_char(char *dst, const char *src, size_t *srcindex)
{
	int	len;

	len = ft_strlen(dst);
	dst = mem_realloc(dst, len + 2);
	dst[len] = src[*srcindex];
	*srcindex += 1;
	dst[len + 1] = '\0';
	return (dst);
}

static char	*handle_expand_prompt_vars(const char *decoded, size_t *i,
		char *result)
{
	char	*sub;
	size_t	j;

	sub = NULL;
	j = 0;
	if (decoded[*i] == '\\')
		return (add_char(result, decoded, i));
	sub = get_prompt_sub(decoded[*i]);
	if (sub)
		result = ft_strjoin(result, sub);
	else
	{
		result = add_char(result, "\\", &j);
		result = add_char(result, decoded, i);
		(*i)--;
	}
	if (decoded[*i])
		(*i)++;
	return (result);
}

static char	*expand_prompt_vars(const char *decoded)
{
	char	*result;
	size_t	i;

	i = 0;
	result = mem_calloc(sizeof(char), 1);
	while (decoded[i])
	{
		if (decoded[i] == '\\')
		{
			i++;
			result = handle_expand_prompt_vars(decoded, &i, result);
			continue ;
		}
		result = add_char(result, decoded, &i);
	}
	return (result);
}

static char	*decode_prompt(const char *ps1)
{
	char	*tmp;
	char	*decoded;

	if (!ps1)
		return (NULL);
	tmp = mem_malloc(ft_strlen(ps1) + 1);
	remove_escape_blocks(ps1, tmp);
	decoded = replace_escape_sequences(tmp);
	if (!decoded)
		return (NULL);
	return (expand_prompt_vars(decoded));
}

char	*get_prompt(void)
{
	t_variable	*v;
	char		*ps1;

	ps1 = PS1;
	v = find_variable("PS1", create_tables(0));
	if (v)
		ps1 = v->value;
	ps1 = ft_strtrim(ps1, "\"'");
	ps1 = decode_prompt(ps1);
	return (ps1);
}

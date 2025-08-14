/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 06:23:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/15 01:21:19 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "expander.h"

static char	*replace_star_unquoted(char *input)
{
	char	*out;
	int		in_sq;
	int		in_dq;
	size_t	j;
	size_t	i;

	if (!input)
		return (NULL);
	out = mem_calloc(ft_strlen(input) + 1, sizeof(char));
	in_sq = 0;
	in_dq = 0;
	j = 0;
	i = -1;
	while (++i < ft_strlen(input))
	{
		if (input[i] == '\'' && !in_dq)
			in_sq = !in_sq;
		else if (input[i] == '"' && !in_sq)
			in_dq = !in_dq;
		else if (input[i] == '*' && !in_sq && !in_dq)
			out[j++] = CTLESC;
		if (out[j - 1] != CTLESC)
			out[j++] = input[i];
	}
	return (out);
}

static t_word	*alloc_param(int *sindex, char *temp, int i,
	int *is_expand)
{
	t_word	*ret;

	if (is_expand)
		*is_expand = 0;
	*sindex = i;
	ret = alloc_word_desc();
	temp = replace_star_unquoted(temp);
	ret->word = temp;
	return (ret);
}

static char	*pass_param(char *str, int *sindex)
{
	int	i;

	if (!str)
		return (0);
	i = *sindex;
	while (str[*sindex] && (ft_isalnum(str[*sindex]) || str[*sindex] == '_'))
		*sindex += 1;
	if (*sindex > i)
		return (ft_substr(str, i, *sindex - i));
	return (NULL);
}

t_word	*param_expand(char *str, int *sindex, int *is_expand,
	int exit_code)
{
	char		*temp1;
	int			i;
	t_variable	*var;

	if (is_expand)
		*is_expand = 1;
	i = *sindex;
	if (str[++i] != '?')
	{
		temp1 = pass_param(str, &i);
		if ((!temp1 || !*temp1) && !(str[i] == '\'' || str[i] == '"'))
			return (alloc_param(sindex, ft_strdup("$"), i, is_expand));
		if ((!temp1 || !*temp1))
			return (alloc_param(sindex, NULL, i, is_expand));
		var = find_variable(temp1, create_tables(0));
		if (var && var->value)
			return (alloc_param(sindex, var->value, i, NULL));
		return (alloc_param(sindex, NULL, i, NULL));
	}
	i += 1 && str[i];
	return (alloc_param(sindex, ft_itoa(exit_code), i, NULL));
}

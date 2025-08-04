/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_param.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 21:38:05 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/04 10:31:33 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "parser_makers.h"

t_word	*param_expand(char *string, int *sindex, int *expanded_something, int exit_code)
{
	char		*temp;
	char		*temp1;
	int			zindex;
	int			t_index;
	char		c;
	t_variable	*var;
	t_word		*ret;

	zindex = *sindex;
	c = string[++zindex];
	temp = NULL;
	ret = NULL;
	if (c == '?')
		 temp = ft_itoa(exit_code);
	else
	{
		temp = NULL;
		t_index = zindex;
		while ((c = string[zindex]) && (ft_isalnum(c) || c == '_'))
			zindex++;
		temp1 = (zindex > t_index) ? ft_substr(string, t_index, zindex - t_index) : NULL;
		if (!temp1 || !*temp1)
		{
			temp = string[zindex] == '\'' || string[zindex] == '"' ? NULL : ft_strdup("$");
			if (expanded_something)
				*expanded_something = 0;
			goto return0;
		}
		var = find_variable(temp1, create_variable_tables(0));
		if (var && var->value)
		{
			temp = var->value;
			goto return0;
		}
		temp = NULL;
		goto return0;
	}
	if (string[zindex])
		zindex++;
	return0:
	*sindex = zindex;
	if (ret == 0)
	{
		ret = alloc_word_desc();
		ret->word = temp;
	}
	return ret;
}

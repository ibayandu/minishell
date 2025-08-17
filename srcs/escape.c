/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 04:29:25 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 04:33:35 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libmem.h"
#include "libft.h"
#include "prompt.h"

size_t	handle_escape_sequence(const char *input, size_t i,
		char *result, size_t *j)
{
	if (input[i] == 'e')
		result[(*j)++] = 27;
	else if (input[i] == '\\')
		result[(*j)++] = '\\';
	else if (input[i] == 'n')
		result[(*j)++] = '\n';
	else
	{
		result[(*j)++] = '\\';
		if (input[i])
			result[(*j)++] = input[i];
	}
	return (i + 1);
}

static void	copy_and_replace(const char *input, char *result)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1])
		{
			i++;
			i = handle_escape_sequence(input, i, result, &j);
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
}

char	*replace_escape_sequences(const char *input)
{
	char	*result;

	result = mem_malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	copy_and_replace(input, result);
	return (result);
}

void	remove_escape_blocks(const char *ps1, char *dst)
{
	size_t	i;
	size_t	j;
	int		in_escape;

	i = 0;
	j = 0;
	in_escape = 0;
	while (ps1[i])
	{
		if (!in_escape && ps1[i] == '\\' && ps1[i + 1] == '[')
		{
			in_escape = 1;
			i += 2;
		}
		else if (in_escape && ps1[i] == '\\' && ps1[i + 1] == ']')
		{
			in_escape = 0;
			i += 2;
		}
		else
			dst[j++] = ps1[i++];
	}
	dst[j] = '\0';
}

char	*get_prompt_sub(char c)
{
	char	*short_pwd;

	if (c == 'u')
		return (get_username());
	else if (c == 'H')
		return (get_hostname_full());
	else if (c == 'w')
		return (get_short_pwd());
	else if (c == 'W')
	{
		short_pwd = get_short_pwd();
		if (!short_pwd)
			return (ft_strdup("?"));
		return (get_basename(short_pwd));
	}
	else if (c == 's')
		return (ft_strdup("minishell"));
	return (NULL);
}

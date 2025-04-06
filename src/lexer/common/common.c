/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:51:38 by ibayandu          #+#    #+#             */
/*   Updated: 2025/04/06 01:01:16 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int	is_operator(char *str)
{
	const char	*op_keywords[] = {"<<-", "&&", "||", ";;", "<<", ">>", "<&",
		">&", "<>", ">|", "{", "}", "!", "|", "(", ")", "<", ">", "&", ";",
		NULL};
	int			i;

	i = -1;
	while (i++ < 19)
		if (strncmp(str, op_keywords[i], strlen(op_keywords[i])) == 0)
			return (strlen(op_keywords[i]));
	return (0);
}

int	skip_whitespace_and_comments(const char *input, int *index, int *in_token)
{
	int	i;

	i = *index;
	if (input[i] == '#')
	{
		while (input[i] && input[i] != '\n')
			i++;
		if (in_token)
			*in_token = 0;
		i++;
		*index = i;
		return (1);
	}
	if (isspace(input[i]))
	{
		if (in_token)
			*in_token = 0;
		while (isspace(input[i]))
			i++;
		*index = i;
		return (1);
	}
	return (0);
}

char	*trim(char *str)
{
	int	start;

	start = 0;
	while (isspace((unsigned char)str[start]))
		start++;
	return (str + start);
}

char	*ft_strdup(const char *str)
{
	size_t	len;
	char	*copy;

	len = strlen(str) + 1;
	copy = (char *)malloc(len);
	if (!copy)
		exit(ENOMEM);
	memcpy(copy, str, len);
	return (copy);
}

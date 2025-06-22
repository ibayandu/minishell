/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:54:41 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/21 20:03:41 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_utils.h"

char	**build_argv(t_word_list *words)
{
	size_t		count;
	t_word_list	*tmp;
	char		**argv;
	size_t		i;

	count = 0;
	tmp = words;
	i = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = ft_malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	tmp = words;
	while (tmp)
	{
		argv[i++] = tmp->word->word;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

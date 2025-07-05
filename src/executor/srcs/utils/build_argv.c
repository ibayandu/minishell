/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:54:41 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/05 18:58:08 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_utils.h"

char	*expand_alias(char *word, t_minishell *minishell)
{
	
}

char	**build_argv(t_word_list *words, t_minishell *minishell)
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
	tmp->word->word = expand_alias(tmp->word->word, minishell);
	while (tmp)
	{
		argv[i++] = tmp->word->word;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_maker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:40:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/13 02:06:25 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"
#include "collector.h"

t_word_list	*make_word_list(t_word *word, t_word_list *wlink)
{
	t_word_list	*temp;

	temp = ft_malloc(sizeof(t_word_list));
	temp->word = word;
	temp->next = wlink;
	return (temp);
}

t_command	*make_bare_simple_command(void)
{
	t_command		*command;
	t_simple_cmd	*temp;

	command = ft_malloc(sizeof(t_command));
	temp = ft_malloc(sizeof(t_simple_cmd));
	command->value.simple = temp;
	temp->flags = 0;
	temp->words = NULL;
	temp->redirects = NULL;
	command->type = CMD_SIMPLE;
	command->redirects = NULL;
	command->flags = 0;
	return (command);
}

t_command	*make_simple_command(t_element *element, t_command *command)
{
	t_redirect	*r;

	if (command == 0)
		command = make_bare_simple_command();
	if (element && element->word)
		command->value.simple->words = make_word_list(element->word,
				command->value.simple->words);
	else if (element->redirect)
	{
		r = element->redirect;
		while (r->next)
			r = r->next;
		r->next = command->value.simple->redirects;
		command->value.simple->redirects = element->redirect;
	}
	return (command);
}

t_command	*clean_simple_command(t_command *command)
{
	if (command->type != CMD_SIMPLE)
		return (NULL);
	else
	{
		command->value.simple->words = ft_revword(command->value.simple->words);
		command->value.simple->redirects = ft_revredir(
				command->value.simple->redirects);
	}
	return (command);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_maker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:40:16 by yzeybek           #+#    #+#             */
/*   Updated: 2025/05/25 09:22:40 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "utils.h"
#include "collector.h"

t_word_list	*make_word_list(t_word *word, t_word_list *wlink)
{
	t_word_list	*temp;

	temp = ft_malloc(sizeof(t_word_list));
	if (!temp)
		return (NULL);
	temp->word = word;
	temp->next = wlink;
	return (temp);
}

t_command	*make_bare_simple_command()
{
	t_command		*command;
	t_simple_cmd	*temp;

	command = ft_malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	temp = ft_malloc(sizeof(t_simple_cmd));
	if (!temp)
		return (NULL);
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
	if (command == 0)
		command = make_bare_simple_command();
	if (element && element->word)
		command->value.simple->words = make_word_list(element->word, command->value.simple->words);
	else if (element->redirect)
	{
		t_redirect	*r = element->redirect;
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
		;// ERROR: not matching type
	else
	{
		command->value.simple->words = ft_revword(command->value.simple->words);
		command->value.simple->redirects = ft_revredir(command->value.simple->redirects);
	}
	return (command);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:17:25 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/02 22:13:54 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser_utils.h"
#include "parser_makers.h"
#include "lexer.h"
#include "expander.h"

t_word_list *list_append(t_word_list *head, t_word_list *tail)
{
	t_word_list	*t_head;

	if (!head)
		return (tail);
	t_head = head;
	while (t_head->next)
		t_head = t_head->next;
	t_head->next = tail;
	return (head);
}

t_word	*copy_word(t_word *w)
{
	t_word	*new_word;

	new_word = make_bare_word(w->word);
	new_word->flags = w->flags;
	return (new_word);
}

t_word_list	*copy_word_list(t_word_list *list)
{
	t_word_list	*new_list;
	t_word_list	*tl;

	new_list = NULL;
	tl = NULL;
	while (list)
	{
		if (!new_list)
		{
			tl = make_word_list(copy_word(list->word), new_list);
			new_list = tl;
		}
		else
		{
			tl->next = make_word_list(copy_word(list->word), NULL);
			tl = tl->next;
		}
		list = list->next;
	}
	return (new_list);
}

void denull_list(t_word_list **list)
{
	t_word_list *cur;
	t_word_list *tmp;

	if (!list || !*list)
		return;
	while (*list && (*list)->word && (*list)->word->word == NULL)
	{
		tmp = *list;
		*list = (*list)->next;
	}
	cur = *list;
	while (cur && cur->next)
	{
		if (cur->next->word && cur->next->word->word == NULL)
		{
			tmp = cur->next;
			cur->next = tmp->next;
		}
		else
			cur = cur->next;
	}
}

char	*expand_alias(char *word, int *expanded_something)
{
	t_variable	*v;

	v = find_variable(word, create_variable_tables(1));
	if (v)
	{
		*expanded_something = 1;
		return (v->value);
	}
	return (word);
}

t_word_list	*shell_expand_word_list(t_word_list *tlist, int *exit_code)
{
	t_word_list	*expanded;
	t_word_list	*new_list;
	t_word_list	*next;
	t_word_list	*temp_list;
	int			expanded_something;

	new_list = NULL;
	while (tlist)
	{
		next = tlist->next;
		expanded_something = 0;
		if (!new_list)
			tlist->word->word = expand_alias(tlist->word->word, &expanded_something);
		expanded = expand_word(tlist->word, 0, &expanded_something, *exit_code);
		if (!expanded)
		{
			tlist->word->word = NULL;
			*exit_code = 1;
			return (NULL);
		}
		if (expanded_something)
			temp_list = word_list_split(expanded);
		else
			temp_list = expanded;
		denull_list(&temp_list);
		expanded = ft_revword(temp_list);
		if (expanded && expanded->word && tlist && tlist->word)
			expanded->word->flags = tlist->word->flags;
		new_list = list_append(expanded, new_list);
		tlist = next;
	}
	if (new_list)
		new_list = ft_revword(new_list);
	return (new_list);
}

t_word_list	*expand_word_list(t_word_list *list, int *exit_code)
{
	t_word_list	*new_list;

	if (!list)
		return (NULL);
	new_list = shell_expand_word_list(copy_word_list(list), exit_code);
	if (new_list)
	{
		new_list = glob_list(new_list);
		if (new_list)
			new_list = ft_revword(new_list);
	}
	return (new_list);
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:17:25 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/06 21:05:46 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"
#include "parser_makers.h"
#include "expander.h"

static t_word_list	*copy_word_list(t_word_list *list)
{
	t_word_list	*new_list;
	t_word		*new_word;
	t_word_list	*tl;

	if (!list)
		return (NULL);
	new_list = NULL;
	new_word = make_bare_word(list->word);
	new_word->flags = list->word->flags;
	tl = make_word_list(new_word, new_list);
	new_list = tl;
	while (list)
	{
		new_word = make_bare_word(list->word);
		new_word->flags = list->word->flags;
		tl->next = make_word_list(new_word, NULL);
		tl = tl->next;
		list = list->next;
	}
	return (new_list);
}

static void denull_list(t_word_list **list)
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

static char	*expand_alias(char *word, int *is_expand)
{
	t_variable	*v;

	*is_expand = 0;
	v = find_variable(word, create_tables(1));
	if (v)
	{
		*is_expand = 1;
		return (v->value);
	}
	return (word);
}

static t_word_list	*shell_expand_word_list(t_word_list *tlist, int *exit_code)
{
	t_word_list	*expanded;
	t_word_list	*new_list;
	t_word_list	*temp_list;
	int			is_expand;

	new_list = NULL;
	while (tlist)
	{
		if (!new_list)
			tlist->word->word = expand_alias(tlist->word->word, &is_expand);
		expanded = expand_word(tlist->word, 0, &is_expand, *exit_code);
		if (!expanded)
		{
			tlist->word->word = NULL;
			*exit_code = 1;
			return (NULL);
		}
		if (is_expand)
			temp_list = word_list_split(expanded);
		else
			temp_list = expanded;
		denull_list(&temp_list);
		expanded = ft_revword(temp_list);
		if (expanded && expanded->word && tlist && tlist->word)
			expanded->word->flags = tlist->word->flags;
		new_list = list_append(expanded, new_list);
		tlist = tlist->next;
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


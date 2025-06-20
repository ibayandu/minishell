/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:17:25 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 15:01:02 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "utils.h"
#include "makers.h"
#include "token.h"
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
			tl = make_word_list(copy_word (list->word), new_list);
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

t_word_list	*shell_expand_word_list(t_word_list *tlist)
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
		expanded = expand_word_internal(tlist->word, 0, 0, &expanded_something);
		if (!expanded /*expanded == &expand_word_error || expanded == &expand_word_fatal */)
		{
			tlist->word->word = NULL;
			// last_command_exit_value = EXECUTION_FAILURE;
			// if (expanded == &expand_word_error)
			// 	exp_jump_to_top_level (DISCARD);
			// else
			// 	exp_jump_to_top_level (FORCE_EOF);
		}
	//	if (expanded_something && !(tlist->word->flags & F_NOSPLIT))
	//		temp_list = word_list_split (expanded);
	//	else
			temp_list = expanded;
		expanded = ft_revword(temp_list);
		new_list = list_append(expanded, new_list);
		tlist = next;
	}
	if (new_list)
		new_list = ft_revword(new_list);
	return (new_list);
}

t_word_list	*expand_word_list(t_word_list *list, int is_redir)
{
	t_word_list	*new_list;

	(void)is_redir;
	if (!list)
		return (NULL);
	new_list = shell_expand_word_list(copy_word_list(list));
	// if (new_list)
	// {
	// 	if (!is_redir)
	// 		new_list = glob_expand_word_list(new_list);
	// 	else
	// 		new_list = dequote_list(new_list);
	// }
	return (new_list);
}


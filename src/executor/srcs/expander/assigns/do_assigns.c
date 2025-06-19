/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_assigns.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 19:43:00 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/17 19:55:22 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "makers.h"
#include "token.h"

t_word_list	*separate_out_assignments(t_word_list *tlist, t_word_list **assign_list)
{
	t_word_list	*vp;
	t_word_list	*lp;

	if (!tlist)
		return (NULL);
	*assign_list = NULL;
	lp = tlist;
	vp = lp;
	while (lp && (lp->word->flags & F_ASSIGNMENT))
	{
		vp = lp;
		lp = lp->next;
	}
	if (lp != tlist)
	{
		*assign_list = tlist;
		vp->next = NULL;
		tlist = lp;
	}
	if (!tlist)
		return (NULL);
	return (tlist);
}

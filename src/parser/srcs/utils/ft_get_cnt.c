/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cnt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:34:36 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 01:03:30 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "lexer.h"

t_cnt_type	ft_get_cnt(void)
{
	t_cnt_type	cnt_type;

	cnt_type = 0;
	if (get_current_token()->token_type == T_AND_IF)
		cnt_type = CNT_AND_AND;
	else if (get_current_token()->token_type == T_OR_IF)
		cnt_type = CNT_OR_OR;
	else if (get_current_token()->token_type == T_NL
		&& get_current_token()->flags != 1)
		cnt_type = CNT_NL;
	else if (get_current_token()->token_type == T_PIPE)
		cnt_type = CNT_PIPE;
	return (cnt_type);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cnt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:34:36 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/20 00:50:16 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "lexer.h"

t_cnt_type	ft_get_cnt(t_token *token)
{
	t_cnt_type	cnt_type;

	cnt_type = 0;
	if (token->token_type == T_AND_IF)
		cnt_type = CNT_AND_AND;
	else if (token->token_type == T_OR_IF)
		cnt_type = CNT_OR_OR;
	else if (token->token_type == T_NL
		&& token->flags != 1)
		cnt_type = CNT_NL;
	else if (token->token_type == T_PIPE)
		cnt_type = CNT_PIPE;
	return (cnt_type);
}

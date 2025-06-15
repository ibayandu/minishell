/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:53:45 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 00:33:33 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "structs.h"
#include "lexer.h"

t_redir_type	ft_get_redir(void)
{
	t_redir_type	redir_type;

	redir_type = 0;
	if (get_current_token()->token_type == T_GREAT
		|| get_current_token()->token_type == T_NUMBER_GREAT)
		redir_type = REDIR_OUTPUT;
	else if (get_current_token()->token_type == T_LESS
		|| get_current_token()->token_type == T_NUMBER_LESS)
		redir_type = REDIR_INPUT;
	else if (get_current_token()->token_type == T_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT)
		redir_type = REDIR_APPEND;
	else if (get_current_token()->token_type == T_DLESS
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		redir_type = REDIR_UNTIL;
	return (redir_type);
}

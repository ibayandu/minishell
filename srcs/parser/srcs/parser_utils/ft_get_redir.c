/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:53:45 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 11:43:42 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser_utils.h"

t_redir_type	ft_get_redir(t_token *token)
{
	t_redir_type	redir_type;

	redir_type = 0;
	if (token->token_type == T_GREAT
		|| token->token_type == T_NUMBER_GREAT)
		redir_type = REDIR_OUTPUT;
	else if (token->token_type == T_LESS
		|| token->token_type == T_NUMBER_LESS)
		redir_type = REDIR_INPUT;
	else if (token->token_type == T_DGREAT
		|| token->token_type == T_NUMBER_DGREAT)
		redir_type = REDIR_APPEND;
	else if (token->token_type == T_DLESS
		|| token->token_type == T_NUMBER_DLESS)
		redir_type = REDIR_UNTIL;
	return (redir_type);
}

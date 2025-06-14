/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:37:31 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/14 02:29:32 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_command	*parse_inputunit(void)
{
	t_command	*cmd;

	parse_newline_list();
	cmd = parse_list();
	if (!cmd)
		return (ft_panic());
	if (get_current_token()->token_type == T_NL)
		return (cmd);
	return (ft_panic());
}


t_command	*parse_compound_list(void)
{
	t_command	*list_cmd;

	parse_newline_list();
	list_cmd = parse_list();
	if (get_current_token()->token_type == T_NL)
	{
		get_next_token();
		parse_newline_list();
	}
	return (list_cmd);
}

t_command	*parse_list(void)
{
	t_command		*left;
	t_command		*right;
	t_cnt_type		cnt_type;

	left = parse_pipeline();
	while (get_current_token()->token_type == T_AND_IF
		|| get_current_token()->token_type == T_OR_IF
		|| get_current_token()->token_type == T_NL)
	{
		if (get_current_token()->token_type == T_AND_IF)
			cnt_type = CNT_AND_AND;
		else if (get_current_token()->token_type == T_OR_IF)
			cnt_type = CNT_OR_OR;
		else
			cnt_type = CNT_NL;
		get_next_token();
		parse_newline_list();
		right = parse_pipeline();
		if (!right)
			return (ft_panic());
		left = command_connect(left, right, cnt_type);
	}
	return (left);
}

void	parse_newline_list(void)
{
	while (get_current_token()->token_type == T_NL)
		get_next_token();
}

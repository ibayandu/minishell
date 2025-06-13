/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:37:31 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/13 02:52:37 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_command	*parse_inputunit(void)
{
	t_command	*cmd;

	cmd = NULL;
	if (get_current_token()->token_type == T_EOF
		|| get_current_token()->token_type == T_NL)
		return (NULL);
	cmd = parse_list();
	if (cmd && !parse_list_terminator())
		return (NULL);
	else if (!cmd && (get_current_token()->token_type != T_EOF
			|| get_current_token()->token_type != T_NL))
		return (NULL);
	return (cmd);
}

t_command	*parse_compound_list(void)
{
	t_command	*list_cmd;

	parse_newline_list();
	list_cmd = parse_list();
	if (!list_cmd)
		return (NULL);
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
	if (!left)
		return (NULL);
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
		if (!left)
			return (NULL);
	}
	return (left);
}

void	parse_newline_list(void)
{
	while (get_current_token()->token_type == T_NL)
		get_next_token();
}

int	parse_list_terminator(void)
{
	if (get_current_token()->token_type == T_NL)
		return (get_next_token(), 1);
	if (get_current_token()->token_type == T_EOF)
		return (1);
	return (0);
}

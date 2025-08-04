/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:37:31 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/03 00:33:16 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "parser_makers.h"
#include "parser_funcs.h"
#include "parser_utils.h"

t_command	*parse_compound_list(int *exit_code)
{
	t_command	*list_cmd;

	parse_newline_list();
	list_cmd = parse_list(exit_code);
	if (!list_cmd)
		return (NULL);
	if (get_current_token()->token_type == T_NL)
		parse_newline_list();
	return (list_cmd);
}

t_command	*parse_list(int *exit_code)
{
	t_command	*left;
	t_command	*right;
	t_cnt_type	cnt_type;

	left = parse_pipeline(exit_code);
	if (!left)
		return (NULL);
	while (ft_get_cnt(get_current_token()))
	{
		cnt_type = ft_get_cnt(get_current_token());
		get_next_token();
		parse_newline_list();
		if (get_current_token()->token_type != T_NL || cnt_type != CNT_NL)
		{
			right = parse_pipeline(exit_code);
			if (!right)
				return (NULL);
			left = command_connect(left, right, cnt_type);
		}
	}
	return (left);
}

void	parse_newline_list(void)
{
	while (get_current_token()->token_type == T_NL
		&& get_current_token()->flags != 1)
		get_next_token();
}

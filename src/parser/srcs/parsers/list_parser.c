/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:37:31 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 00:40:47 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_command	*parse_inputunit(t_minishell *minishell)
{
	t_command	*cmd;

	parse_newline_list();
	if (get_current_token()->token_type == T_NL)
		return (NULL);
	cmd = parse_list(minishell);
	if (!cmd)
		return (NULL);
	if (get_current_token()->token_type == T_NL)
		return (cmd);
	return (ft_panic(), NULL);
}

t_command	*parse_compound_list(t_minishell *minishell)
{
	t_command	*list_cmd;

	parse_newline_list();
	list_cmd = parse_list(minishell);
	if (!list_cmd)
		return (NULL);
	if (get_current_token()->token_type == T_NL)
		parse_newline_list();
	return (list_cmd);
}

t_command	*parse_list(t_minishell *minishell)
{
	t_command		*left;
	t_command		*right;
	t_cnt_type		cnt_type;

	left = parse_pipeline(minishell);
	if (!left)
		return (NULL);
	while (ft_get_cnt())
	{
		cnt_type = ft_get_cnt();
		get_next_token();
		parse_newline_list();
		if (get_current_token()->token_type != T_NL)
		{
			right = parse_pipeline(minishell);
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

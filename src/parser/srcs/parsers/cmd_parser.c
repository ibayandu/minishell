/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:34 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/14 02:32:38 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_element	*parse_simple_command_element(void)
{
	t_element	*elem;

	elem = ft_calloc(sizeof(t_element), 1);
	if (get_current_token()->token_type == T_WORD)
	{
		elem->word = make_word(get_current_token()->value,
				get_current_token()->flags);
		get_next_token();
	}
	else
		elem->redirect = parse_redirection();
	return (elem);
}

t_command	*parse_simple_command(void)
{
	t_command		*cmd;
	int				element_parsed;
	t_element		*element;

	cmd = make_bare_simple_command();
	element_parsed = 0;
	while (1)
	{
		if (get_current_token()->token_type == T_WORD || ft_get_redir())
		{
			element = parse_simple_command_element();
			element_parsed = 1;
			cmd = make_simple_command(element, cmd);
		}
		else
			break ;
	}
	if (!element_parsed)
		return (ft_panic());
	return (clean_simple_command(cmd));
}

t_command	*parse_command(void)
{
	t_command	*cmd;
	t_command	*sub_cmd;
	t_redirect	*redirects;

	cmd = NULL;
	if (get_current_token()->token_type == T_LPARANTHESE)
	{
		get_next_token();
		sub_cmd = parse_compound_list();
		if (!sub_cmd)
			return (ft_panic());
		cmd = make_subshell_command(sub_cmd);
		if (!consume_token(T_RPARANTHESE))
			return (ft_panic());
		if (ft_get_redir())
		{
			redirects = parse_redirection_list();
			cmd->redirects = ft_revredir(redirects);
		}
	}
	else
		cmd = parse_simple_command();
	return (cmd);
}

t_command	*parse_pipeline(void)
{
	t_command	*left;
	t_command	*right;

	left = parse_command();
	while (get_current_token()->token_type == T_PIPE)
	{
		get_next_token();
		parse_newline_list();
		right = parse_command();
		if (!right)
			return (ft_panic());
		left = command_connect(left, right, CNT_PIPE);
	}
	return (left);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:34 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/13 02:52:05 by yzeybek          ###   ########.tr       */
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
		if (!elem->word)
			return (NULL);
		get_next_token();
	}
	else
	{
		elem->redirect = parse_redirection();
		if (!elem->redirect)
			return (NULL);
	}
	return (elem);
}

t_command	*parse_simple_command(void)
{
	t_command		*cmd;
	int				element_parsed;
	t_token_type	token_type;
	t_element		*element;
	t_element		*temp_element;

	cmd = make_bare_simple_command();
	if (!cmd)
		return (NULL);
	element_parsed = 0;
	while (1)
	{
		token_type = get_current_token()->token_type;
		if (token_type == T_WORD || token_type == T_GREAT
			|| token_type == T_LESS || token_type == T_DGREAT
			|| token_type == T_DLESS || token_type == T_NUMBER_DGREAT
			|| token_type == T_NUMBER_DLESS || token_type == T_NUMBER_LESS
			|| token_type == T_NUMBER_GREAT)
		{
			element = parse_simple_command_element();
			if (!element)
			{
				if (!element_parsed)
					ft_panic();
				return (NULL);
			}
			element_parsed = 1;
			temp_element = element;
			cmd = make_simple_command(temp_element, cmd);
			if (!cmd)
				return (NULL);
		}
		else
			break ;
	}
	if (!element_parsed)
		return (ft_panic());
	cmd = clean_simple_command(cmd);
	return (cmd);
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
		if (!cmd)
			return (ft_panic());
		if (!consume_token(T_RPARANTHESE))
			return (ft_panic());
		redirects = parse_redirection_list();
		if (redirects)
			cmd->redirects = ft_revredir(redirects);
	}
	else
	{
		cmd = parse_simple_command();
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

t_command	*parse_pipeline(void)
{
	t_command	*left;
	t_command	*right;

	left = parse_command();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_PIPE)
	{
		get_next_token();
		right = parse_command();
		if (!right)
			return (ft_panic());
		left = command_connect(left, right, CNT_PIPE);
		if (!left)
			return (NULL);
	}
	return (left);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:34 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/19 22:39:50 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"

t_command	*parse_inputunit()
{
	t_command	*res;

	res = NULL;
	if (get_current_token()->token_type == T_EOF)
		return (NULL);
	res = parse_simple_list();
	if (!res)
	{
		if (get_current_token()->token_type == T_EOF)
			return (NULL); // ERROR:
	}
	if (!consume_token(T_EOF))
		return (NULL); //ERROR:
	return (res);
}

t_redirect	*parse_redirection()
{

}

t_element	*parse_simple_command_element()
{

}

t_redirect	*parse_redirection_list()
{

}

t_command	*parse_simple_command()
{
	t_command		*cmd;
	int				element_parsed;
	t_token_type	token_type;
	t_element		*element;
	t_element		temp_element;

	cmd = make_bare_simple_command();
	if (!cmd)
		return (NULL);
	element_parsed = 0;
	while (1)
	{
		token_type = get_current_token()->token_type;
		if (token_type == T_WORD || token_type == T_GREAT || token_type == T_LESS
		|| token_type == T_DGREAT || token_type == T_DLESS) //HACK: Understand the which are the words should include here
		{
			element = parse_simple_command_element();
			if (!element)
			{
				if (!element_parsed)
					;//ERROR
				return (NULL);
			}
			element_parsed = 1;
			temp_element = *element;
			cmd = make_simple_command(temp_element,cmd);
			if (!cmd)
				return (NULL);
		}
		else
			break;
	}
	if (!element_parsed)
	{
		//error
		return (NULL);
	}
	cmd = clean_simple_command(cmd);
	return (cmd);
}

t_command	*parse_command()
{
	t_command	*cmd;
	t_command	*sub_cmd;
	t_redirect	*redirects;

	cmd = NULL;
	if (get_current_token()->token_type == T_LPARANTHESE)
	{
		get_next_token();
		sub_cmd = parse_list();
		if (!sub_cmd)
			return (NULL);
		cmd = make_subshell_command(sub_cmd);
		if (!cmd)
			return (NULL);
		if (!consume_token(T_RPARANTHESE))
			return (NULL);
		redirects = parse_redirection_list();
		if (!redirects)
			return (NULL);
	}
	else
	{
		cmd = parse_simple_command();
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

t_command	*parse_list()
{

}

t_command	*parse_simple_list()
{
	t_command	*left;
	t_command	*right;
	t_cnt_type	*connector;

	left = parse_pipeline();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_AND_IF
		|| get_current_token()->token_type == T_OR_IF)
	{
		if (get_current_token()->token_type == T_AND_IF)
			connector = CNT_AND_AND;
		else
			connector = CNT_OR_OR;
		get_next_token();
		right = parse_pipeline();
		if (!right)
			return (NULL);
		left = command_connect(left, right, connector);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_command	*parse_pipeline()
{
	t_command	*left;
	t_command	*right;
	t_cnt_type	*connector;

	left = parse_command();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_PIPE)
	{
		get_next_token();
		right = parse_command();
		if (!right)
			return (NULL);
		left = command_connect(left, right, CNT_PIPE);
		if (!left)
			return (NULL);
	}
	return (left);
}

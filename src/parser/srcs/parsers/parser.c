/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:34 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/25 12:30:00 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

t_command	*parse_inputunit()
{
	t_command	*cmd;

	cmd = NULL;
	if (get_current_token()->token_type == T_EOF || get_current_token()->token_type == T_NL)
		return (NULL);
	cmd = parse_list();
	if (cmd && !parse_list_terminator())
		return (NULL);
	else if (!cmd && get_current_token()->token_type != T_EOF && get_current_token()->token_type != T_NL)
		return (NULL);
	return (cmd);
}

t_redirect	*parse_redirection()
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;


	source = NULL;
	if (get_current_token()->token_type == T_NUMBER_LESS
		|| get_current_token()->token_type == T_NUMBER_GREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		source = make_word(ft_itoa(ft_atoi(get_current_token()->value)), 0);
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
	else
		return (NULL); // ERROR: Expected redirection operator
	get_next_token();
	if (!source)
	{
		if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
			source = make_word("1", 0);
		else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
			source = make_word("0", 0);
	}
	if (get_current_token()->token_type == T_WORD)
	{
		dest = make_word(get_current_token()->value, get_current_token()->flags);
		if (!dest)
			return (NULL);
		get_next_token();
	}
	else
		return (NULL); // ERROR: Expected WORD (filename/delimiter) after redirection operator
	redirect = make_redirection(source, redir_type, dest);
	if (!redirect)
		return (NULL);
	if (redir_type == REDIR_UNTIL)
		make_here_document(redirect);
	return (redirect);

}

t_element	*parse_simple_command_element()
{
	t_element	*elem;

	elem = ft_calloc(sizeof(t_element), 1);
	if (!elem)
		return (NULL); // ERROR: Malloc failed for t_element
	if (get_current_token()->token_type == T_WORD)
	{
		elem->word = make_word(get_current_token()->value, get_current_token()->flags);
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

t_redirect	*parse_redirection_list()
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection();
	if (!current)
		return (NULL);
	head = current;
	current = parse_redirection();
	while (current != NULL)
	{
		current->next = head;
		head = current;
		current = parse_redirection();
	}
	return (head);
}

t_command	*parse_simple_command()
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
		if (token_type == T_WORD || token_type == T_GREAT || token_type == T_LESS
		|| token_type == T_DGREAT || token_type == T_DLESS
		|| token_type == T_NUMBER_DGREAT || token_type == T_NUMBER_DLESS
		|| token_type == T_NUMBER_LESS || token_type == T_NUMBER_GREAT)
		{
			element = parse_simple_command_element();
			if (!element)
			{
				if (!element_parsed)
					;// ERROR: Expected simple command element
				return (NULL);
			}
			element_parsed = 1;
			temp_element = element;
			cmd = make_simple_command(temp_element, cmd);
			if (!cmd)
				return (NULL);
		}
		else
			break;
	}
	if (!element_parsed)
	{
		// ERROR: Empty simple command parsed
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
		sub_cmd = parse_compound_list();
		if (!sub_cmd)
			return (NULL); // ERROR: Expected list after '(' in subshell
		cmd = make_subshell_command(sub_cmd);
		if (!cmd)
			return (NULL);
		if (!consume_token(T_RPARANTHESE))
			return (NULL); // ERROR: Expected ')' after list in subshell
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

t_command	*parse_list()
{
	t_command		*left;
	t_command		*right;
	t_cnt_type		cnt_type;

	left = parse_pipeline();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_AND_IF || get_current_token()->token_type == T_OR_IF || get_current_token()->token_type == T_NL)
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
			return (NULL); // ERROR: Expected pipeline after connector in list
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

int parse_list_terminator(void)
{
	if (get_current_token()->token_type == T_NL)
		return (get_next_token(), 1);
	if (get_current_token()->token_type == T_EOF)
		return (1);
	return (0);
}

t_command	*parse_pipeline()
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
			return (NULL); // ERROR: Expected command after '|' in pipeline
		left = command_connect(left, right, CNT_PIPE);
		if (!left)
			return (NULL);
	}
	return (left);
}

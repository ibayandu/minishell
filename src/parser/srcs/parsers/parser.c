/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 14:28:34 by ibayandu          #+#    #+#             */
/*   Updated: 2025/05/21 22:22:47 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

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
			return (NULL); // ERROR: Expected simple_list or EOF at inputunit start
	}
	if (!consume_token(T_EOF))
		return (NULL);
	return (res);
}

t_redirect	*parse_redirection()
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;


	source = NULL;
	dest = NULL;
	t_token *token = get_current_token();
	(void)token;
	if (get_current_token()->token_type == T_WORD && get_current_token()->flags & F_NUMBER )
	{
		source = make_word(get_current_token()->value, get_current_token()->flags);
		get_next_token();
	}
	if (get_current_token()->token_type == T_GREAT)
		redir_type = REDIR_OUTPUT;
	else if (get_current_token()->token_type == T_LESS)
		redir_type = REDIR_INPUT;
	else if (get_current_token()->token_type == T_DGREAT)
		redir_type = REDIR_APPEND;
	else if (get_current_token()->token_type == T_DLESS)
		redir_type = REDIR_UNTIL;
	else
		return (NULL); // ERROR: Expected redirection operator after NUMBER
	get_next_token();
	if (!source)
	{
		if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
			source = make_word("1", F_NUMBER);
        else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
			source = make_word("0", F_NUMBER);
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
	t_element		*elem;

	elem = ft_malloc(sizeof(t_element));
	if (!elem)
		return (NULL); // ERROR: Malloc failed for t_element
	elem->word = NULL;
	elem->redirect = NULL;
	t_token *token = get_current_token();
	(void)token;
	if (get_current_token()->token_type == T_WORD && !(get_current_token()->flags & F_NUMBER))
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
	t_element		temp_element;

	cmd = make_bare_simple_command();
	if (!cmd)
		return (NULL);
	element_parsed = 0;
	while (1)
	{
		token_type = get_current_token()->token_type;
		if (token_type == T_WORD || token_type == T_GREAT || token_type == T_LESS
		|| token_type == T_DGREAT || token_type == T_DLESS)
		{
			element = parse_simple_command_element();
			if (!element)
			{
				if (!element_parsed)
					;// ERROR: Expected simple command element
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
		sub_cmd = parse_list();
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

t_command	*parse_list()
{
	t_command		*left;
	t_command		*right;
	t_cnt_type		cnt_type;

	left = parse_pipeline();
	if (!left)
		return (NULL);
	while (get_current_token()->token_type == T_AND_IF || get_current_token()->token_type == T_OR_IF)
	{
		if (get_current_token()->token_type == T_AND_IF)
			cnt_type = CNT_AND_AND;
		else
			cnt_type = CNT_OR_OR;
		get_next_token();
		right = parse_pipeline();
		if (!right)
			return (NULL); // ERROR: Expected pipeline after connector in list
		left = command_connect(left, right, cnt_type);
		if (!left)
			return (NULL);
	}
	return (left);
}

t_command	*parse_simple_list()
{
	t_command	*left;
	t_command	*right;
	t_cnt_type	connector;

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
			return (NULL); // ERROR: Expected pipeline after connector in simple_list
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:33:07 by yzeybek           #+#    #+#             */
/*   Updated: 2025/06/16 19:24:50 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "makers.h"
#include "parsers.h"
#include "utils.h"

static void	push_heredoc(t_redirect *r, t_minishell *minishell)
{
	if (minishell->need_here_doc >= HEREDOC_MAX)
	{
		ft_putendl_fd("minishell: maximum here-document count exceeded",
			STDERR_FILENO);
		ft_free();
		exit(2);
	}
	minishell->redir_stack[minishell->need_here_doc++] = r;
}

t_redirect	*parse_redirection(t_minishell *minishell)
{
	t_redirect		*redirect;
	t_word			*source;
	t_word			*dest;
	t_redir_type	redir_type;

	source = NULL;
	redir_type = ft_get_redir();
	if (get_current_token()->token_type == T_NUMBER_LESS
		|| get_current_token()->token_type == T_NUMBER_GREAT
		|| get_current_token()->token_type == T_NUMBER_DGREAT
		|| get_current_token()->token_type == T_NUMBER_DLESS)
		source = make_word(ft_itoa(ft_atoi(get_current_token()->value)), 0);
	else if (redir_type == REDIR_OUTPUT || redir_type == REDIR_APPEND)
		source = make_word("1", 0);
	else if (redir_type == REDIR_INPUT || redir_type == REDIR_UNTIL)
		source = make_word("0", 0);
	if (get_next_token()->token_type == T_WORD)
		dest = make_word(get_current_token()->value,
				get_current_token()->flags);
	else
		return (ft_panic(), NULL);
	redirect = make_redirection(source, redir_type, dest);
	if (get_next_token() && redir_type == REDIR_UNTIL)
		push_heredoc(redirect, minishell);
	return (redirect);
}

t_redirect	*parse_redirection_list(t_minishell *minishell)
{
	t_redirect	*head;
	t_redirect	*current;

	current = parse_redirection(minishell);
	if (!current)
		return (NULL);
	head = current;
	while (1)
	{
		if (!ft_get_redir())
			break ;
		current = parse_redirection(minishell);
		if (!current)
			return (NULL);
		current->next = head;
		head = current;
	}
	return (head);
}

void	gather_here_documents(t_minishell *minishell)
{
	int	r;

	r = 0;
	while (minishell->need_here_doc > 0)
	{
		make_here_document (minishell->redir_stack[r++]);
		minishell->need_here_doc--;
		minishell->redir_stack[r - 1] = 0;
	}
}

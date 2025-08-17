/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 17:02:38 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/16 23:18:27 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "parser_utils.h"
#include "parser_funcs.h"
#include "parser.h"

t_command	*parse_inputunit(int *exit_code)
{
	t_command	*cmd;

	parse_newline_list();
	if (get_current_token()->token_type == T_NL)
		return (NULL);
	cmd = parse_list(exit_code);
	if (!cmd)
		return (NULL);
	if (get_current_token()->token_type == T_NL)
		return (cmd);
	return (ft_panic(get_current_token(), exit_code), NULL);
}

int	ft_heredoc(int *exit_code)
{
	pid_t		pid;
	char		**paths;
	int			status;
	t_redirect	**redir_stack;

	status = 1;
	redir_stack = push_heredoc(NULL, 0);
	if (!*redir_stack)
		return (0);
	pid = gather_here_documents_fd(&paths, redir_stack);
	if (pid < 0)
		return (1);
	if (waitpid(pid, &status, 0) < 0)
		status = 1;
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*exit_code = WTERMSIG(status) + 128;
	if (!status)
		status = read_heredoc_inputs(paths, redir_stack);
	push_heredoc(NULL, 1);
	return (status);
}

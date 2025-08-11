/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_connect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:50 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/11 04:26:34 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libmem.h"
#include "libft.h"
#include "executor.h"
#include "exec_funcs.h"
#include "parser_makers.h"

static int	execute_if(t_connect_cmd *connect, int *exit_code)
{
	int	status;

	status = execute_command(connect->first, exit_code);
	if (status == 0 && connect->type == CNT_AND_AND)
		return (execute_command(connect->second, exit_code));
	else if (status != 0 && connect->type == CNT_OR_OR)
		return (execute_command(connect->second, exit_code));
	return (status);
}

static void	execute_pipe_left(t_command *cmd, t_redirect *redirects,
		int pipefd[2], int *exit_code)
{
	t_redirect	*r;
	t_redirect	*temp;

	r = make_redirection(make_bare_word("1"), REDIR_OUTPUT,
			make_bare_word(ft_itoa(pipefd[0])), pipefd[1]);
	temp = cmd->redirects;
	while (temp && temp->next)
		temp = temp->next;
	if (temp)
		temp->next = redirects;
	else
		cmd->redirects = redirects;
	if (cmd->type == CMD_CONNECT && cmd->value.connection->type == CNT_PIPE)
	{
		temp = cmd->value.connection->second->redirects;
		cmd->value.connection->second->redirects = r;
		r->next = temp;
		r = make_redirection(make_bare_word(""), REDIR_IGNORE,
				make_bare_word(ft_itoa(pipefd[0])), pipefd[1]);
	}
	temp = cmd->redirects;
	cmd->redirects = r;
	r->next = temp;
	execute_command(cmd, exit_code);
}

static void	execute_pipe_right(t_command *cmd, t_redirect *redirects,
		int pipefd[2], int *exit_code)
{
	t_redirect	*r;
	t_redirect	*temp;

	temp = cmd->redirects;
	while (temp && temp->next)
		temp = temp->next;
	if (temp)
		temp->next = redirects;
	r = make_redirection(make_bare_word("0"), REDIR_INPUT,
			make_bare_word(ft_itoa(pipefd[1])), pipefd[0]);
	temp = cmd->redirects;
	cmd->redirects = r;
	r->next = temp;
	execute_command(cmd, exit_code);
	close(pipefd[0]);
	close(pipefd[1]);
}

static int	execute_pipe(t_connect_cmd *connect, int recursion,
		t_redirect *redirects, int *exit_code)
{
	int		pipefd[2];
	int		status;

	if (!recursion && pipe(pipefd) == -1)
		return (1);
	if (recursion && connect->first->type == CMD_CONNECT
		&& connect->first->value.connection->type == CNT_PIPE)
		return (execute_pipe(connect->first->value.connection,
				recursion + 1, redirects, exit_code));
	else if (connect->first->type == CMD_CONNECT
		&& connect->first->value.connection->type == CNT_PIPE)
		recursion = execute_pipe(connect->first->value.connection,
				recursion + 1, redirects, exit_code);
	else if (recursion)
		return (recursion + 1);
	execute_pipe_left(connect->first, redirects, pipefd, exit_code);
	execute_pipe_right(connect->second, redirects, pipefd, exit_code);
	recursion += 1 * recursion;
	while ((!redirects || !redirects->flags) && recursion--)
		wait(&status);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

int	execute_connect(t_connect_cmd *connect, t_redirect *redirects,
		int *exit_code)
{
	if (connect->type == CNT_AND_AND || connect->type == CNT_OR_OR)
		return (execute_if(connect, exit_code));
	else if (connect->type == CNT_PIPE)
		return (execute_pipe(connect, 0, redirects, exit_code));
	else if (connect->type == CNT_NL)
	{
		*exit_code = execute_command(connect->first, exit_code);
		*exit_code = execute_command(connect->second, exit_code);
		return (*exit_code);
	}
	return (1);
}

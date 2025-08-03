/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_connect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:50 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 01:45:00 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmem.h"
#include "executor.h"
#include "exec_funcs.h"

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

static void	execute_pipe_left_side(t_command *cmd, int pipefd[2],
		int *exit_code)
{
	int	status;

	discard_signals();
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	status = execute_command(cmd, exit_code);
	if (status && errno == EPIPE)
		perror("minishell");
	mem_free();
	exit(status);
}

static void	execute_pipe_right_side(t_command *cmd, int pipefd[2],
		int *exit_code)
{
	int	status;

	discard_signals();
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	status = execute_command(cmd, exit_code);
	if (status && errno == EPIPE)
		perror("minishell");
	mem_free();
	exit(status);
}

static int	execute_pipe(t_connect_cmd *connect, int *exit_code)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 == 0)
		execute_pipe_left_side(connect->first, pipefd, exit_code);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_right_side(connect->second, pipefd, exit_code);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

int	execute_connect(t_connect_cmd *connect, int *exit_code)
{
	if (connect->type == CNT_AND_AND || connect->type == CNT_OR_OR)
		return (execute_if(connect, exit_code));
	else if (connect->type == CNT_PIPE)
		return (execute_pipe(connect, exit_code));
	else if (connect->type == CNT_NL)
	{
		*exit_code = execute_command(connect->first, exit_code);
		*exit_code = execute_command(connect->second, exit_code);
		return (*exit_code);
	}
	return (1);
}

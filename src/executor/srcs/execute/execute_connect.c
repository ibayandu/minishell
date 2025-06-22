/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_connect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:50 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 05:01:21 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "executor.h"

static int	execute_if(t_connect_cmd *connect)
{
	int	status;

	status = execute_command(connect->first);
	if (status == 0 && connect->type == CNT_AND_AND)
		return (execute_command(connect->second));
	else if (status != 0 && connect->type == CNT_OR_OR)
		return (execute_command(connect->second));
	return (status);
}

static void	execute_pipe_left_side(t_command *cmd, int pipefd[2])
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_command(cmd));
}

static void	execute_pipe_right_side(t_command *cmd, int pipefd[2])
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_command(cmd));
}

static int	execute_pipe(t_connect_cmd *connect)
{
	int		pipefd[2];
	int		status;
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	pid1 = fork();
	if (pid1 == 0)
		execute_pipe_left_side(connect->first, pipefd);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_right_side(connect->second, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

int	execute_connect(t_connect_cmd *connect)
{
	if (connect->type == CNT_AND_AND || connect->type == CNT_OR_OR)
		return (execute_if(connect));
	else if (connect->type == CNT_PIPE)
		return (execute_pipe(connect));
	return (1);
}

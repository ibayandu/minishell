/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_connect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:50 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 13:22:13 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "executor.h"

static int	execute_if(t_connect_cmd *connect, t_minishell *minishell)
{
	int	status;

	status = execute_command(connect->first, minishell);
	if (status == 0 && connect->type == CNT_AND_AND)
		return (execute_command(connect->second, minishell));
	else if (status != 0 && connect->type == CNT_OR_OR)
		return (execute_command(connect->second, minishell));
	return (status);
}

static void	execute_pipe_left_side(t_command *cmd, int pipefd[2], t_minishell *minishell)
{
	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[1]);
	exit(execute_command(cmd, minishell));
}

static void	execute_pipe_right_side(t_command *cmd, int pipefd[2], t_minishell *minishell)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	exit(execute_command(cmd, minishell));
}

static int	execute_pipe(t_connect_cmd *connect, t_minishell *minishell)
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
		execute_pipe_left_side(connect->first, pipefd, minishell);
	pid2 = fork();
	if (pid2 == 0)
		execute_pipe_right_side(connect->second, pipefd, minishell);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else
		return (1);
}

int	execute_connect(t_connect_cmd *connect, t_minishell *minishell)
{
	if (connect->type == CNT_AND_AND || connect->type == CNT_OR_OR)
		return (execute_if(connect, minishell));
	else if (connect->type == CNT_PIPE)
		return (execute_pipe(connect, minishell));
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:19:19 by yzeybek           #+#    #+#             */
/*   Updated: 2025/08/17 11:28:16 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "libmem.h"
#include "exec_utils.h"

int	pipe_handle_stat(int status, int count)
{
	if (count && isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

int	pipe_count_wait(t_connect_cmd *connect)
{
	t_connect_cmd	*tmp;
	int				wait_count;

	wait_count = 0;
	tmp = connect;
	while (++wait_count && tmp && tmp->first->type == CMD_CONNECT
		&& tmp->first->value.connection->type == CNT_PIPE)
		tmp = tmp->first->value.connection;
	return (wait_count);
}

void	push_pipe(int pid, int *pids)
{
	int	i;

	i = 0;
	while (pids[i])
		i++;
	if (pid)
		pids[i] = pid;
}

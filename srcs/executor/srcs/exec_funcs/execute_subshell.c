/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/04 10:24:26 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "libmem.h"
#include "exec_utils.h"
#include "exec_funcs.h"
#include "executor.h"

int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects,
					int *exit_code)
{
	pid_t	pid;
	int		status;
	int		res;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(redirects, exit_code))
		{
			mem_free();
			exit(1);
		}
		res = execute_command(subshell->command, exit_code);
		mem_free();
		exit(res);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

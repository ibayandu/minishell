/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/21 19:35:37 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		apply_redirections(redirects);
		exit(execute_command(subshell->command));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

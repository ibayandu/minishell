/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/22 07:43:44 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects, t_minishell *minishell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		apply_redirections(redirects, minishell);
		exit(execute_command(subshell->command, minishell));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

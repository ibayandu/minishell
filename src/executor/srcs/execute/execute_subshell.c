/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:43:39 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	execute_subshell(t_subshell_cmd *subshell, t_redirect *redirects,
		t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	int		res;

	pid = fork();
	if (pid == 0)
	{
		if (apply_redirections(redirects, minishell))
		{
			ft_free();
			exit(1);
		}
		res = execute_command(subshell->command, minishell);
		ft_free();
		exit(res);
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

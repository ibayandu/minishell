/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:59:24 by ibayandu          #+#    #+#             */
/*   Updated: 2025/06/28 10:40:06 by yzeybek          ###   ########.tr       */
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
		if (apply_redirections(redirects, minishell))
		{
			ft_free();
			exit(1);
		}
		exit(execute_command(subshell->command, minishell));
	}
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

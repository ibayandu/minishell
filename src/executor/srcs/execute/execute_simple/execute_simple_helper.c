/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 20:23:42 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/12 20:42:17 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_simple.h"

static void	is_directory(char *filename)
{
	struct stat	sb;
	int			fd;
	char		*joined_str;

	if (ft_strchr(filename, '/'))
	{
		fd = open(filename, O_RDONLY, 0644);
		if (fd >= 0 && (fstat(fd, &sb) == 0) && S_ISDIR(sb.st_mode))
		{
			joined_str = ft_strjoin("minishell: ", filename);
			ft_putendl_fd(ft_strjoin(joined_str, ": Is a directory"),
				STDERR_FILENO);
			ft_free();
			exit(126);
		}
	}
}

static void	handle_redirections(t_redirect *redirects, t_minishell *minishell)
{
	if (redirects)
	{
		if (apply_redirections(redirects, minishell))
		{
			ft_free();
			exit(1);
		}
	}
}

static void	handle_command_error(char *cmd)
{
	if (ft_strchr(cmd, '/') || errno == EACCES)
		perror(ft_strjoin("minishell: ", cmd));
	else
		ft_putendl_fd(ft_strjoin(cmd, ": command not found"), STDERR_FILENO);
	ft_free();
	if (errno == EACCES)
		exit(126);
	exit(127);
}

void	child_process(t_simple_cmd *cmd, t_redirect *redirects,
		t_minishell *minishell)
{
	char *const	*argv;

	argv = build_argv(cmd->words);
	setup_signals_exec();
	handle_redirections(redirects, minishell);
	handle_redirections(cmd->redirects, minishell);
	if (!argv || !argv[0])
	{
		ft_free();
		exit(0);
	}
	ft_execvp(argv[0], argv, minishell);
	is_directory(argv[0]);
	handle_command_error(argv[0]);
}

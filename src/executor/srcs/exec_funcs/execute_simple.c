/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:31 by ibayandu          #+#    #+#             */
/*   Updated: 2025/08/03 01:34:53 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "libft.h"
#include "libmem.h"
#include "exec_funcs.h"
#include "exec_utils.h"

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
			mem_free();
			exit(126);
		}
	}
}

static void	handle_redirections(t_redirect *redirects, int *exit_code)
{
	if (redirects)
	{
		if (apply_redirections(redirects, exit_code))
		{
			mem_free();
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
	mem_free();
	if (errno == EACCES)
		exit(126);
	exit(127);
}

void	child_process(t_simple_cmd *cmd, t_redirect *redirects, int *exit_code)
{
	char *const	*argv;

	argv = build_argv(cmd->words);
	setup_signals_exec();
	handle_redirections(redirects, exit_code);
	handle_redirections(cmd->redirects, exit_code);
	if (!argv || !argv[0])
	{
		mem_free();
		exit(0);
	}
	ft_execvp(argv[0], argv);
	is_directory(argv[0]);
	handle_command_error(argv[0]);
}

int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, int *exit_code)
{
	int		fds[3];
	int		ret;
	pid_t	pid;
	int		status;

	cmd->words = expand_word_list(cmd->words, exit_code);
	ret = execute_builtin(cmd, fds, exit_code);
	if (ret != -1)
		return (ret);
	discard_signals();
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		child_process(cmd, redirects, exit_code);
	if (waitpid(pid, &status, 0) < 0)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

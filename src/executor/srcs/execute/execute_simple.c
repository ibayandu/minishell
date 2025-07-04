/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzeybek <yzeybek@student.42.com.tr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 18:58:31 by ibayandu          #+#    #+#             */
/*   Updated: 2025/07/04 20:43:31 by yzeybek          ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <termios.h>
#include <errno.h>
#include "builtin.h"
#include "expander.h"
#include "execute.h"

void	handle_child(int sig)
{
	(void)sig;
	ft_free();
	exit(130);
}

static void	is_directory(char *filename)
{
	struct stat	sb;
	int			fd;

	if (ft_strchr(filename, '/'))
	{
		fd = open(filename, O_RDONLY, 0644);
		if (fd >= 0 && (fstat (fd, &sb) == 0) && S_ISDIR (sb.st_mode))
		{
			ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: ", filename), ": Is a directory"), STDERR_FILENO);
			ft_free();
			exit(126);
		}
	}
}

static void	child_process(t_simple_cmd *cmd, t_redirect *redirects, t_minishell *minishell)
{

	char *const *argv = build_argv(cmd->words);

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_child);
	if (redirects)
		if (apply_redirections(redirects, minishell))
		{
			ft_free();
			exit(1);
		}
	if (cmd->redirects)
		if (apply_redirections(cmd->redirects, minishell))
		{
			ft_free();
			exit(1);
		}
	if (!argv || !argv[0])
	{
		ft_free();
		exit(0);
	}
	ft_execvp(argv[0], argv, minishell);
	is_directory(argv[0]);
	if (ft_strchr(argv[0], '/') || errno == EACCES)
		perror(ft_strjoin("minishell: ", argv[0]));
	else
		ft_putendl_fd(ft_strjoin(ft_strjoin("minishell: ", argv[0]), ": command not found"), STDERR_FILENO);
	ft_free();
	if (errno == EACCES)
		exit(126);
	exit(127);
}

static int	wait_for_child(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	if (WIFSTOPPED(status))
		return (WSTOPSIG(status));
	return (1);
}

int	store_fds(int fds[3])
{
	fds[0] = dup(STDIN_FILENO);
	if (fds[0] < 0)
		return (1);
	fds[1] = dup(STDOUT_FILENO);
	if (fds[1] < 0)
		return (1);
	fds[2] = dup(STDERR_FILENO);
	if (fds[2] < 0)
		return (1);
	return (0);
}

int	restore_fds(int fds[3])
{
	if (dup2(fds[0], STDIN_FILENO) < 0)
		return (1);
	if (close(fds[0]) < 0)
		return (1);
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return (1);
	if (close(fds[1]) < 0)
		return (1);
	if (dup2(fds[2], STDERR_FILENO) < 0)
		return (1);
	if (close(fds[2]) < 0)
		return (1);
	return (0);
}

int	execute_simple(t_simple_cmd *cmd, t_redirect *redirects, t_minishell *minishell)
{
	int		fds[3];
	int		ret;
	pid_t	pid;

	cmd->words = expand_word_list(cmd->words, minishell);
	if (cmd->words && cmd->words->word && cmd->words->word->word && is_builtin(cmd->words->word->word))
	{
		if (store_fds(fds))
			return (1);
		if (apply_redirections(cmd->redirects,minishell))
			return (restore_fds(fds), 1);
		ret = run_builtin(cmd, minishell);
		if (restore_fds(fds))
			return (1);
		return (ret);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (1);
	else if (pid == 0)
		child_process(cmd, redirects, minishell);
	else
		return (wait_for_child(pid));
	return (1);
}

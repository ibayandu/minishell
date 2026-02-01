/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibayandu <ibayandu@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 00:53:15 by ibayandu          #+#    #+#             */
/*   Updated: 2026/01/11 10:02:28 by ibayandu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "expander.h"
#include "init.h"
#include "lexer.h"
#include "libft.h"
#include "libgnl.h"
#include "libmem.h"
#include "minishell.h"
#include "parser.h"
#include "prompt.h"
#include <dirent.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global değişken - PATH komutlarını cache'lemek için
static char	**g_cached_commands = NULL;

static int	*set_code(int code)
{
	static int	exit_code = 0;

	if (code >= 0)
		exit_code = code;
	return (&exit_code);
}

static void	handle_sigint(int sig)
{
	(void)sig;
	if (isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	set_code(130);
}

// PATH'teki komutları tarayan yardımcı fonksiyon
static int	add_commands_from_dir(const char *dir_path, char **commands,
		int count, int max)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*full_path;
	int				i;
	int				duplicate;

	dir = opendir(dir_path);
	if (!dir)
		return (count);
	while ((entry = readdir(dir)) != NULL && count < max)
	{
		if (entry->d_name[0] != '.')
		{
			full_path = ft_strjoin(ft_strjoin(dir_path, "/"), entry->d_name);
			if (access(full_path, X_OK) == 0)
			{
				i = 0;
				duplicate = 0;
				while (i < count)
				{
					if (ft_strcmp(commands[i], entry->d_name) == 0)
					{
						duplicate = 1;
						break ;
					}
					i++;
				}
				if (!duplicate)
					commands[count++] = ft_strdup(entry->d_name);
			}
		}
	}
	closedir(dir);
	return (count);
}

// PATH'teki tüm komutları döndüren fonksiyon - SADECE BİR KERE ÇAĞRILIR
static void	init_path_commands(void)
{
	char	*path_env;
	char	**paths;
	int		i;
	int		count;

	if (g_cached_commands) // Zaten yüklenmişse tekrar yükleme
		return ;
	path_env = find_variable("PATH", create_tables(0))->value;
	if (!path_env)
		return ;
	paths = ft_split(path_env, ':');
	if (!paths)
		return ;
	g_cached_commands = mem_malloc(sizeof(char *) * 5000);
	if (!g_cached_commands)
		return ;
	count = 0;
	i = 0;
	while (paths[i] && count < 5000)
	{
		count = add_commands_from_dir(paths[i], g_cached_commands, count,
				5000);
		i++;
	}
	g_cached_commands[count] = NULL;
}

char	*command_generator(const char *text, int state)
{
	static char	*builtin_cmds[] = {"alias", "cd", "echo", "env", "exit", 
			"export", "pwd", "unalias", "unset", NULL};
	static int	list_index;
	static int	len;
	static int	checking_builtins;
	char		*name;

	if (!state)
	{
		list_index = 0;
		len = ft_strlen(text);
		checking_builtins = 1;
	}
	// Önce built-in komutları kontrol et
	if (checking_builtins)
	{
		while ((name = builtin_cmds[list_index]))
		{
			list_index++;
			if (ft_strncmp(name, text, len) == 0)
				return (ft_strdup(name));
		}
		checking_builtins = 0;
		list_index = 0;
	}
	// PATH komutlarını kontrol et (cache'den)
	if (g_cached_commands)
	{
		while (g_cached_commands[list_index])
		{
			name = g_cached_commands[list_index];
			list_index++;
			if (ft_strncmp(name, text, len) == 0)
				return (ft_strdup(name));
		}
	}
	return (NULL);
}

char	**get_command_matches(const char *text, int start, int end)
{
	(void)end;
	if (start == 0)
		return (rl_completion_matches(text, command_generator));
	return (NULL);
}

char	*ft_repl(void)
{
	char	*line;

	line = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handle_sigint);
	if (isatty(STDIN_FILENO))
		line = mem_absorb(readline(get_prompt()));
	else
		line = gnl_one(STDIN_FILENO);
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			write(STDERR_FILENO, "exit\n", 5);
		mem_free();
		exit(0);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	if (*line)
		add_history(line);
	return (line);
}

int	main(void)
{
	t_command	*cmd;
	char		*cmdline;
	extern char	**environ;

	initialize_shell_variables(environ, set_code(-1));
	rl_attempted_completion_function = get_command_matches;
	init_path_commands(); // PATH komutlarını BAŞLANGIŞTA bir kere yükle
	cmdline = ft_repl();
	while (cmdline)
	{
		if (init_lexer(ft_strjoin(cmdline, "\n")))
		{
			cmd = parse_inputunit(set_code(-1));
			if (!ft_heredoc(set_code(-1)) && cmd)
				set_code(execute_command(cmd, set_code(-1)));
		}
		cmdline = ft_repl();
	}
	rl_clear_history();
	mem_free();
	return (*set_code(-1));
}
